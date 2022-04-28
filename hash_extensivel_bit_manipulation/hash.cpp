#include <bits/stdc++.h>
#include <string>
#include <math.h>

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

using namespace std;

#define SIZEBUCKET 32

class Diretorio
{
public:
	fstream out;
	typedef struct Vinho
	{
		int id, ano_colheita;
		string rotulo, tipo;

		void createVinho(int id_vinho, int ano, string r, string t)
		{
			id = id_vinho;
			ano_colheita = ano;
			rotulo = r;
			tipo = t;
		}
	} Vinho;

	class Bucket
	{
	public:
		int profLocal, size = 0;
		int chaves[SIZEBUCKET];
		fstream arquivo;
		string file_name;

		bool estaVazio()
		{
			if (size == 0)
				return true; // verificando se o bucket está vazio
			return false;
		}

		bool estaCheio()
		{
			if (size >= SIZEBUCKET)
				return true; // verificando se o bucket está cheio
			return false;
		}

		bool hasVinho(int i, int ano_colheita)
		{
			if (chaves[i] == ano_colheita)
				return true;
			return false;
		}

		Vinho read_file(int i)
		{
			Vinho v;

			arquivo.open(file_name, ios_base::in);

			string str_change;
			string vinho_id, rotulo, ano_colheita, tipo;

			int j = 0;
			while (arquivo.good())
			{
				getline(arquivo, vinho_id, ',');
				getline(arquivo, rotulo, ',');
				getline(arquivo, ano_colheita, ',');
				getline(arquivo, tipo);
				if (j == i)
				{
					str_change = vinho_id + ',' + rotulo + ',' + ano_colheita + ',' + tipo + '\n';
					v.createVinho(stoi(vinho_id), stoi(ano_colheita), rotulo, tipo);
					break;
				}
				j++;
			}

			arquivo.close();

			// cópia do conteúdo
			arquivo.open(file_name, ios_base::in | ios_base::binary | std::ios_base::app);
			stringstream vinho;

			vinho << arquivo.rdbuf();
			string str = vinho.str();
			size_t pos = str.find(str_change);
			str.replace(pos, string(str_change).length(), "");
			arquivo.close();

			remove(file_name.c_str());

			arquivo.open(file_name, ios_base::out | ios_base::binary | std::ios_base::app);
			arquivo << str;
			arquivo.close();
			return v;
		}

		Vinho get_vinho(int i)
		{
			Vinho v = read_file(i);
			for (int j = i; j < size - 1; j++)
				chaves[j] = chaves[j + 1];
			return v;
		}

		int removeEmMassa(int ano_colheita)
		{
			if (size == 0)
				return 0;

			int count = 0, i = 0;
			while (i < size)
			{
				if (chaves[i] == ano_colheita)
				{
					get_vinho(i);
					size--;
					count++;
				}
				else
					i++;
			}
			return count;
		}

		int coutVinhos(int ano_colheita)
		{
			int j = 0;
			for (int i = 0; i < size; i++)
				if (chaves[i] == ano_colheita)
					j++;
			return j;
		}

		void inserir_vinho(Vinho v)
		{
			string text(to_string(v.id) + "," + v.rotulo + "," + to_string(v.ano_colheita) + "," + v.tipo);

			arquivo.open(file_name, std::ios_base::app);
			arquivo << text << endl;
			arquivo.close();

			chaves[size] = v.ano_colheita;
			size++;
		}
	};

	int profGlobal, qtdPonteiros;

	typedef struct Hash
	{
		Bucket *bucket;
	} Hash;

	Hash *hashs;

	Diretorio(int pGlobal)
	{
		profGlobal = pGlobal;
		qtdPonteiros = pow(2, pGlobal);
		hashs = new Hash[qtdPonteiros];

		for (int i = 0; i < qtdPonteiros; i++)
			hashs[i] = criar_hash(criar_bucket(profGlobal, i));
	}

	bool verifyFileExists(int posHashAtual)
	{
		string name = ("buckets/bucket_" + to_string(posHashAtual) + ".txt");
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
		string newName = ("buckets/bucket_" + to_string(posHashAtual) + ".txt");
		rename(b->file_name.c_str(), newName.c_str());
		b->file_name = newName;
		b->arquivo.open(b->file_name, ios_base::out | ios_base::binary | std::ios_base::app);
		b->arquivo.close();
	}

	Bucket *criar_bucket(int profundidade, int n)
	{
		Bucket *b = new Bucket;
		b->profLocal = profundidade;
		b->file_name = "buckets/bucket_" + to_string(n) + ".txt";
		b->arquivo.open(b->file_name, ios_base::out | ios_base::binary | std::ios_base::app);
		b->arquivo.close();
		return b;
	}

	Hash criar_hash(Bucket *bucket)
	{
		Hash h;
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

	Hash *duplicarDiretorio()
	{
		int novaProfGlobal = profGlobal + 1, posHash = 0, novaQtdPonteiros = pow(2, novaProfGlobal);
		Hash *novosHash = new Hash[novaQtdPonteiros];

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

			if (hashs[posHash].bucket->estaCheio() && hashs[posHash].bucket->hasVinho(i, bucketAtual->chaves[i]))
				hashs[posHash].bucket = criar_bucket(novaProfundidadeLocal, posHash);

			insert_recursive(bucketAtual->get_vinho(i), posHash);
			bucketAtual->size--;
		}
	}

	void insert_recursive(Vinho v, int posHash)
	{
		Bucket *bucketEncontrado = hashs[posHash].bucket;

		bucketEncontrado->inserir_vinho(v);

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

	void insert(int ano)
	{
		Vinho v;
		ifstream vinhos_csv("../vinhos.csv");

		int posHash = searchFunctionHash(profGlobal, ano);
		out.open("out.txt", std::ios_base::app);

		string vinho_id, rotulo, ano_colheita, tipo;
		getline(vinhos_csv, vinho_id); // pegar primeira linha do arquivo

		int qtd = 0, profL = 0;
		while (vinhos_csv.good())
		{
			getline(vinhos_csv, vinho_id, ',');
			getline(vinhos_csv, rotulo, ',');
			getline(vinhos_csv, ano_colheita, ',');
			getline(vinhos_csv, tipo);

			if (stoi(ano_colheita) == ano)
			{
				v.createVinho(stoi(vinho_id), ano, rotulo, tipo);
				insert_recursive(v, posHash);
				qtd++;
			}
		}

		if (qtd != 0)
			profL = hashs[posHash].bucket->profLocal;

		string text("INC:" + to_string(ano) + "/" + to_string(qtd) + ',' + to_string(profGlobal) + ',' + to_string(profL));
		out << text << endl;
		vinhos_csv.close();
		out.close();
	}

	void remover(int ano)
	{
		out.open("out.txt", std::ios_base::app);

		int posHash = searchFunctionHash(profGlobal, ano), posPairHash = 0, novaProfundidade = 0;
		int qtd = hashs[posHash].bucket->removeEmMassa(ano), profL = 0;

		if (qtd != 0)
			profL = hashs[posHash].bucket->profLocal;
		string text("REM:" + to_string(ano) + "/" + to_string(qtd) + ',' + to_string(profGlobal) + ',' + to_string(profL));
		out << text << endl;
		out.close();

		if (qtd == 0 || qtdPonteiros <= 4)
			return;

		if (hashs[posHash].bucket->estaVazio())
		{
			novaProfundidade = hashs[posHash].bucket->profLocal - 1;

			posPairHash = searchFunctionHash(novaProfundidade, posHash);
			remove(hashs[posHash].bucket->file_name.c_str());

			hashs[posHash].bucket = hashs[posPairHash].bucket;
			hashs[posHash].bucket->file_name = hashs[posPairHash].bucket->file_name;
			hashs[posPairHash].bucket->profLocal = novaProfundidade;
			hashs[posHash].bucket->profLocal = novaProfundidade;
		}
	}

	void search(int ano)
	{
		out.open("out.txt", std::ios_base::app);
		int posHash = searchFunctionHash(profGlobal, ano);
		int qtd = hashs[posHash].bucket->coutVinhos(ano);

		string text("BUS:" + to_string(ano) + "/" + to_string(qtd));
		out << text << endl;
		out.close();
	}
};

int main()
{
	Diretorio d(2);
	fstream in("in.txt");

	string op, ano;
	int y = 0;
	while (in.good())
	{
		getline(in, op, ':');
		getline(in, ano);

		y = stoi(ano);

		if (op == "INC")
			d.insert(y);
		else if (op == "REM")
			d.remover(y);
		else if (op == "BUS=")
			d.search(y);
	}

	return 0;
}