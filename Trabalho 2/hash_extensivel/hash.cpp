#include <bits/stdc++.h>
#include <math.h>
#include <fstream>
#include<direct.h>

#include "esquemas.cpp"

using namespace std;

#define SIZEBUCKET 40

template <typename TypeTable>
class Hash
{
public:
	fstream out;
	string folder_name;

	class Bucket
	{
	public:
		int profLocal, size = 0;
		int chaves[SIZEBUCKET];
		fstream arquivo;
		string file_name;


		bool estaCheio()
		{
			if (size >= SIZEBUCKET)
				return true; // verificando se o bucket está cheio
			return false;
		}

		bool hasValue(int i, int chave)
		{
			if (chaves[i] == chave)
				return true;
			return false;
		}

		TypeTable read_file(int i)
		{
			
			string str_change;
			TypeTable type_t = type_t.read(file_name, i, &str_change);

			if(type_t.chave == -1) return type_t;

			// cópia do conteúdo
			arquivo.open(file_name, ios_base::in);
			stringstream esquema;

			esquema << arquivo.rdbuf();
			string str = esquema.str();
			size_t pos = str.find(str_change);
			
			str.replace(pos, string(str_change).length(), "");

			arquivo.close();

			remove(file_name.c_str());
			

			arquivo.open(file_name, ios_base::out | std::ios_base::app);
			arquivo << str;
			arquivo.close();
			return type_t;
		}

		TypeTable get(int i)
		{
			TypeTable v = read_file(i);	
			if(v.chave == -1) return v;
			for (int j = i; j < size - 1; j++)
				chaves[j] = chaves[j + 1];
			return v;
		}

		void insert_value_on_disk(TypeTable v)
		{

			string text = v.get_cols(v);

			arquivo.open(file_name, std::ios_base::app);
			arquivo << text << endl;
			arquivo.close();

			chaves[size] = v.chave;
			size++;
		}
	};

	int profGlobal, qtdPonteiros;

	typedef struct HashS
	{
		Bucket *bucket;
	} HashS;

	HashS *hashs;

	Hash(int pGlobal, string nfolder)
	{
		profGlobal = pGlobal;
		qtdPonteiros = pow(2, pGlobal);
		hashs = new HashS[qtdPonteiros];
		folder_name = "buckets_"+nfolder;
		mkdir(folder_name.c_str())
			
		for (int i = 0; i < qtdPonteiros; i++)
			hashs[i] = criar_hash(criar_bucket(profGlobal, i));
	}

	bool verifyFileExists(int posHashAtual)
	{
		string name = (folder_name+"/bucket_" + to_string(posHashAtual) + ".txt");
		ifstream arquivo;
		arquivo.open(name);
		if (arquivo.fail())
		{
			return false;
		}
		return true;
	}

	void copy_file(Bucket *b, int posHashAtual)
	{
		string newName = (folder_name+"/bucket_" + to_string(posHashAtual) + ".txt");
		rename(b->file_name.c_str(), newName.c_str());
		b->file_name = newName;
		b->arquivo.open(b->file_name, ios_base::out);
		b->arquivo.close();
	}

	Bucket *criar_bucket(int profundidade, int n)
	{
		Bucket *b = new Bucket;
		b->profLocal = profundidade;
		b->file_name = folder_name+"/bucket_" + to_string(n) + ".txt";
		b->arquivo.open(b->file_name, ios_base::out);
		b->arquivo.close();
		return b;
	}

	HashS criar_hash(Bucket *bucket)
	{
		HashS h;
		h.bucket = bucket;
		return h;
	};

	int searchFunctionHash(int profundidade, int i)
	{
		int j = pow(2, profundidade) - 1;
		int k = i & j;
		if (k > qtdPonteiros)
			return -1;
		return k;
	}

	HashS *duplicarDiretorio()
	{
		int novaProfGlobal = profGlobal + 1, posHash = 0, novaQtdPonteiros = pow(2, novaProfGlobal);
		HashS *novosHash = new HashS[novaQtdPonteiros];

		for (int i = 0; i < novaQtdPonteiros; i++)
		{
			if (i < qtdPonteiros)
				novosHash[i] = hashs[i];
			else
			{
				posHash = searchFunctionHash(profGlobal, i);
				novosHash[i] = criar_hash(hashs[posHash].bucket);
			}
		}

		qtdPonteiros = novaQtdPonteiros;
		profGlobal = novaProfGlobal;
		delete hashs;
		return novosHash;
	}

	void dividir_bucket(int posHashAtual)
	{
		Bucket *bucketAtual = hashs[posHashAtual].bucket;

		int novaProfundidadeLocal = bucketAtual->profLocal + 1, posHash = 0;
		bucketAtual->profLocal = novaProfundidadeLocal;

		int i = 0;
		while (i < bucketAtual->size)
		{
			posHash = searchFunctionHash(novaProfundidadeLocal, bucketAtual->chaves[i]);
			if (posHash == posHashAtual)
			{
				i++;
				continue;
			}
			if (!verifyFileExists(posHashAtual))
				copy_file(bucketAtual, posHashAtual);


			if (hashs[posHash].bucket->estaCheio() && hashs[posHash].bucket->hasValue(i, bucketAtual->chaves[i]))
				hashs[posHash].bucket = criar_bucket(novaProfundidadeLocal, posHash);

			insert_recursive(bucketAtual->get(i), posHash);
			bucketAtual->size--;
		}
	}

	void insert_recursive(TypeTable v, int posHash)
	{
		if(v.chave == -1) return;
		Bucket *bucketEncontrado = hashs[posHash].bucket;

		bucketEncontrado->insert_value_on_disk(v);

		if (bucketEncontrado->estaCheio())
		{
			if (bucketEncontrado->profLocal < profGlobal)
				dividir_bucket(posHash);
			else
			{
				hashs = duplicarDiretorio();
				dividir_bucket(posHash);
			}
		}
	}

	void insert(string fn)
	{
		TypeTable v;
		ifstream csv_file(fn);

		int posHash = 0;

		string first_line;
		getline(csv_file, first_line); // pegar primeira linha do arquivo

		int qtd = 0, profL = 0;
		while (csv_file.good())
		{
			v = v.read_csv(csv_file);
			if (v.chave != -1)
			{
				posHash = searchFunctionHash(profGlobal, v.chave);
				insert_recursive(v, posHash);
				qtd++;
			}
		}
	}
};
