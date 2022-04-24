#include <bits/stdc++.h>
#include <string>
#include <math.h>

using namespace std;

#define SIZEBUCKET 4

class Diretorio
{
public:
	typedef struct Vinho
	{
		int id;
		int ano_colheita;
		string rotulo;
		string tipo;
	} Vinho;

	class Bucket
	{
	public:
		int profLocal, size = 0;
		Vinho vinhos[SIZEBUCKET]; // cada bucket cabe 32 registros

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

		void inserir_vinho(Vinho v)
		{
			vinhos[size] = v;
			size++;
		}
	};

	typedef struct Hash
	{
		int *lsb;
		Bucket *bucket;

	} Hash;

	int profGlobal, qtdPonteiros;
	Hash *hashs;

	Diretorio(int pGlobal)
	{
		profGlobal = pGlobal;
		qtdPonteiros = pow(2, pGlobal);
		hashs = new Hash[qtdPonteiros];

		for (int i = 0; i < qtdPonteiros; i++)
		{ // inicializando buckets
			hashs[i] = criar_hash(criar_bucket(profGlobal), i);
		}
	}

	Bucket *criar_bucket(int profundidade)
	{
		Bucket *b = new Bucket;
		b->profLocal = profundidade;

		return b;
	}

	Hash criar_hash(Bucket *bucket, int i)
	{
		Hash h;
		h.lsb = toBinary(i, false, profGlobal);
		h.bucket = bucket;
		return h;
	};

	int *inicializar_vetor(int size)
	{ // preenchendo vetor com 0
		int *newVetor = new int[size];
		for (int i = 0; i < size; i++)
			newVetor[i] = 0;
		return newVetor;
	}

	int *toBinary(int value, bool getLsbs, int profundidade)
	{
		int *bits = inicializar_vetor(32);
		int bitSize = 0;

		int *lsb = new int[profundidade];

		int i = 0;

		while (value != 0)
		{
			bits[i] = value % 2;
			value /= 2;
			i++;
			bitSize++;
		}

		if (bitSize < profundidade)
			bitSize = profundidade;

		if (getLsbs)
			for (i = 0; i < profundidade; i++)
				lsb[(profundidade - 1) - i] = bits[i];
		else
			for (i = bitSize - 1; i >= 0; i--)
				lsb[(bitSize - 1) - i] = bits[i];

		return lsb;
	}

	Vinho createVinho(int id, int ano_colheita, string rotulo, string tipo)
	{
		Vinho vinho;
		vinho.id = id;
		vinho.ano_colheita = ano_colheita;
		vinho.rotulo = rotulo;
		vinho.tipo = tipo;
		return vinho;
	}

	int searchFunctionHash(int *lsbVinho)
	{

		bool encontrado;
		for (int i = 0; i < qtdPonteiros; i++)
		{
			encontrado = true;
			for (int j = 0; j < profGlobal; j++)
			{
				if (hashs[i].lsb[j] != lsbVinho[j])
				{
					encontrado = false;
					continue;
				}
			}
			if (encontrado)
				return i;
		}
		return -1;
	}

	Hash *duplicarDiretorio()
	{
		int novaProfGlobal = profGlobal + 1, posHash = 0;
		int novaQtdPonteiros = pow(2, novaProfGlobal);

		Hash *novosHash = new Hash[novaQtdPonteiros];
		int *lsbFr = nullptr;

		for (int i = 0; i < novaQtdPonteiros; i++)
		{
			if (i < qtdPonteiros)
			{
				novosHash[i] = hashs[i];
				novosHash[i].lsb = toBinary(i, false, novaProfGlobal);
			}
			else
			{
				lsbFr = toBinary(i, true, novaProfGlobal - 1);
				posHash = searchFunctionHash(lsbFr);
				novosHash[i] = criar_hash(hashs[posHash].bucket, i);
			}
		}

		qtdPonteiros = novaQtdPonteiros;
		profGlobal = novaProfGlobal;
		delete hashs;
		return novosHash;
	}

	void dividir_bucket(int posHashAtual)
	{
		Bucket bucketAtual = *(hashs[posHashAtual].bucket);

		int novaProfundidadeLocal = bucketAtual.profLocal + 1, posHash = 0;

		hashs[posHashAtual].bucket = criar_bucket(novaProfundidadeLocal);

		int *lsbVinho = nullptr;

		for (int i = 0; i < SIZEBUCKET; i++)
		{
			lsbVinho = toBinary(bucketAtual.vinhos[i].ano_colheita, true, profGlobal);

			posHash = searchFunctionHash(lsbVinho);

			if (hashs[posHash].bucket->estaCheio())
			{
				hashs[posHash].bucket = criar_bucket(novaProfundidadeLocal);
			}

			hashs[posHash].bucket->inserir_vinho(bucketAtual.vinhos[i]);
		}
	}

	void percorrerBuckets()
	{
		for (int i = 0; i < qtdPonteiros; i++)
		{
			for (int k = 0; k < profGlobal; k++)
				cout << hashs[i].lsb[k];
			cout << " - global: " << profGlobal << " local: " << hashs[i].bucket->profLocal << endl;
			for (int j = 0; j < hashs[i].bucket->size; j++)
			{
				cout << hashs[i].bucket->vinhos[j].ano_colheita << ' ';
			}
			cout << endl;
		}
	}

	void insert(int id, int ano_colheita, string rotulo, string tipo)
	{
		Vinho v = createVinho(id, ano_colheita, rotulo, tipo);

		int *lsbVinho = toBinary(ano_colheita, true, profGlobal);
		int posHash = searchFunctionHash(lsbVinho);

		if (posHash == -1)
			return;

		Hash hashEncontrado = hashs[posHash];

		Bucket *bucketEncontrado = hashEncontrado.bucket;

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
};

int main()
{
	Diretorio d(1);

	d.insert(1, 16, "vinho da isa", "branco");
	d.insert(1, 4, "vinho da isa", "branco");
	d.insert(1, 6, "vinho da isa", "branco");
	d.insert(1, 22, "vinho da isa", "branco");
	d.insert(1, 24, "vinho da isa", "branco");
	d.insert(1, 10, "vinho da isa", "branco");
	d.insert(1, 31, "vinho da isa", "branco");
	d.insert(1, 7, "vinho da isa", "branco");
	d.insert(1, 9, "vinho da isa", "branco");
	d.insert(1, 20, "vinho da isa", "branco");
	d.insert(1, 26, "vinho da isa", "branco");
	d.percorrerBuckets();

	return 0;
}