// contr.cpp : База данных "Населенный пункт"
//


#include <iostream>
#include <fstream>
#include <Windows.h>
#include <algorithm>
#include <vector>
#include <codecvt>

using namespace std;

struct tRec {			// строка записи БД
	char fio[32];
	char street[20];
	short ndom;
	short nkv;
	char date[8];
};

struct tNode {		// узел дерева содержащий запись  БД
	struct tRec data;
	struct tNode *left;
	struct tNode *right;
	int balance; // 1- L > R; 0 - L=R; -1 - L<R;
};


struct strNode {
	string data;
	struct strNode *left;
	struct strNode *right;
	int balance;
};

struct peopleNode {
	unsigned int key;
	string fio;
	struct peopleNode* left;
	struct peopleNode* right;
	boolean balance;
};

typedef struct tNode* pNode;
typedef struct strNode* streetNode;
typedef struct peopleNode* plNode;

vector <tRec> base;	


// функция сравнения двух массивов
int compareArr(char A[], char B[], int size) {
	int i = 0;
	int j = 0;
	while (i < size || j < size) {
		if (A[i] == ' ') i++;
		if (B[j] == ' ') j++;
		if (A[i] == B[j]) {
			i++;
			j++;
		}
		else {
			if (A[i] > B[j]) {
				return 1;
			}
			else return -1;
		}
	}
	return 0;
}




// подпрогамма сортировки массива методом Хоара
void sortArray(int mArr[], int first, int last) {
	int mid, tmp;
	int f = first;
	int l = last;

	mid = mArr[(f + l) / 2];

	do
	{
		while (mArr[f] < mid) {
			f++;
		}
		while (mArr[l] > mid) {
			l--;
		}
		if (f <= l)
		{
			tmp = mArr[f];
			mArr[f] = mArr[l];
			mArr[l] = tmp;
			f++;
			l--;
		}
	} while (f < l);

	if (first < l) sortArray(mArr, first, l);
	if (f < last) sortArray(mArr, f, last);
}


//Подпрограмма построения АВЛ дерева по названиям улиц
void addStreet(struct tRec rec, streetNode &p, boolean *h) {
	streetNode p1 = new strNode;
	streetNode p2 = new strNode;
	if (p == NULL) {
		p = new strNode;
		string s(rec.street, 20);
		p->data = s;
		p->left = NULL;
		p->right = NULL;
		p->balance = 0;
		*h = true;
	}
	else
	{
		string rs(rec.street, 20);
		if (p->data > rs) {
			addStreet(rec, p->left, h);
			if (*h == true) {
				switch (p->balance) {
				case 1: {
					p->balance = 0;
					*h = false;
					break;
				}
				case 0: {
					p->balance--;
					break;
				}
				case -1: {
					p1 = p->left;
					if (p1->balance == -1) {
						p->left = p1->right;
						p1->right = p;
						p->balance = 0;
						p = p1;
					}
					else {
						p2 = p1->right;
						p1->right = p2->left;
						p2->left = p1;
						p->left = p2->right;
						p2->right = p;
						if (p2->balance == -1) {
							++p->balance;
						}
						else {
							p->balance = 0;
						}
						if (p2->balance = 1) {
							--p1->balance;
						}
						else {
							p1->balance = 0;
						}
						p = p2;
					}
					p->balance = 0;
					*h = false;
					break;
				}
				default: break;
				}
			}
		}
		else
		{
			if (p->data < rs)
			{
				addStreet(rec, p->right, h);
				if (*h == true) {
					switch (p->balance) {
					case -1: {										// поддерево в целом не выросло
						p->balance = 0;
						*h = false;
						break;
					}
					case 0: {											// поддерево выросло допустимо для АВЛ дерева
						++p->balance;
						break;
					}
					case 1: {											// требуется балансировка
						p1 = p->right;
						if (p1->balance == 1) {			//  RR поворот
							p->right = p1->left;
							p1->left = p;
							p->balance = 0;
							p = p1;
						}
						else {											// RL поворот
							p2 = p1->left;
							p1->left = p2->right;
							p2->right = p1;
							p->right = p2->left;
							p2->left = p;
							if (p2->balance == 1) { --p->balance; }
							else p->balance = 0;
							if (p2->balance == -1) { ++p1->balance; }
							else p1->balance = 0;
							p = p2;
						}
						p->balance = 0;
						*h = false;
						break;
					}
					default: break;
					}
				}
			}	else *h = false;
		}
	}
}

//Подпрограмма построения АВЛ дерева по ФИО
void fioTree(struct tRec rec, pNode &p, boolean *h) {
	pNode p1 = new tNode;
	pNode p2 = new tNode;
	if (p == NULL) {
		p = new tNode;
		p->data = rec;
		p->left = NULL;
		p->right = NULL;
		p->balance = 0;
		*h = true;
	}
	else														
	{
		if (compareArr(p->data.fio, rec.fio, 32) == 1) {
			fioTree(rec, p->left, h);
			if (*h == true) {
				switch (p->balance) {
				case 1: {
					p->balance = 0;
					*h = false;
					break;
				}
				case 0: {
					p->balance--;
					break;
				}
				case -1: {
					p1 = p->left;
					if (p1->balance == -1) {
						p->left = p1->right;
						p1->right = p;
						p->balance = 0;
						p = p1;
					}
					else {													
						p2 = p1->right;
						p1->right = p2->left;
						p2->left = p1;
						p->left = p2->right;
						p2->right = p;
						if (p2->balance == -1) {
							++p->balance;
						}
						else {
							p->balance = 0;
						}
						if (p2->balance = 1) {
							--p1->balance;
						}
						else {
							p1->balance = 0;
						}
						p = p2;
					}
					p->balance = 0;
					*h = false;
					break;
				}
				default: break;
				}
			}
		}
		else
		{
			if (compareArr(p->data.fio, rec.fio, 32) == -1) {
				fioTree(rec, p->right, h);
				if (*h == true) {								// правая ветвь выросла
					switch (p->balance) {
					case -1: {										// поддерево в целом не выросло
						p->balance = 0;
						*h = false;
						break;
					}
					case 0: {											// поддерево выросло допустимо для АВЛ дерева
						++p->balance;
						break;
					}
					case 1: {											// требуется балансировка
						p1 = p->right;
						if (p1->balance == 1) {			//  RR поворот
							p->right = p1->left;
							p1->left = p;
							p->balance = 0;
							p = p1;
						}
						else {											// RL поворот
							p2 = p1->left;
							p1->left = p2->right;
							p2->right = p1;
							p->right = p2->left;
							p2->left = p;
							if (p2->balance == 1) { --p->balance; }
							else p->balance = 0;
							if (p2->balance == -1) { ++p1->balance; }
							else p1->balance = 0;
							p = p2;
						}
						p->balance = 0;
						*h = false;
						break;
					}
					default: break;
					}
				}
			}	else h = false;
		} 
	}
}

//Подпрограмма построения АВЛ дерева по фамилии
void fTree(tRec rec, plNode &p, boolean *h) {
	plNode p1 = new peopleNode;
	plNode p2 = new peopleNode;
	string s(rec.fio, 32);
	if (p == NULL) {
		p = new peopleNode;
		p->fio = s;
		p->key = rec.ndom;
		p->left = NULL;
		p->right = NULL;
		p->balance = 0;
		*h = true;
	}
	else
	{
		if (p->fio > s) {

		}
	}
}



// вывод на экран списка улиц
void printStreet(streetNode p) {
	if (p != NULL) {
		if (p->left) printStreet(p->left);
		cout << p->data << endl;
		if (p->right) printStreet(p->right);
	}
}


// подпроргамма подсчета количества улиц
int streetCount(streetNode p) {
	if (p == NULL) {
		return 0;
	}
	else {
		return 1 + streetCount(p->left) + streetCount(p->right);
	}
}

// Функция поиска вершины с заданным ключом key
//struct tNode * searchNode(struct tNode *p, int key) {
//	if (p == NULL) {
//		return NULL;
//	}
//	else {
//		if (p->data == key) {
//			return p;
//		}
//		if (p->data > key) {
//			p = searchNode(p->left, key);
//			return p;
//		}
//		if (p->data < key) {
//			p = searchNode(p->right, key);
//			return p;
//		}
//	}
//
//
//}



void mMenu()
{
	pNode root = NULL;
	streetNode street = NULL;

	int key = 0;
	int k = 0;
	int *M = NULL;
	struct tNode *searchRez = NULL;
	boolean h = false;
	int num = 3;


	const char *fName = "base4.dat";
	tRec frec;

	ifstream fbase;
	fbase.open(fName, ios::binary | ios::in);
	if (!fbase.is_open()) {
		cout << "ОШибка открытия файла" << endl;
	}
	else {
		base.resize(0);
		tRec buf;
		while (!fbase.read((char*)&buf, sizeof(tRec)).eof()) {
			OemToAnsiBuff(buf.fio, buf.fio, 32);
			OemToAnsiBuff(buf.street, buf.street, 20);
			tRec *rec = new tRec;

			for (int i = 0; i < 32; i++) {
				rec->fio[i] = buf.fio[i];
			}
			for (int i = 0; i < 20; i++) {
				rec->street[i] = buf.street[i];
			}
			rec->ndom = buf.ndom;
			rec->nkv = buf.nkv;
			for (int i = 0; i < 8; i++) {
				rec->date[i] = buf.date[i];
			}

			base.push_back(*rec);

			/*for (int i = 0; i < base.size(); i++) {
				string fio(base[i].fio, 32);
				string street(base[i].street, 20);
				string date(base[i].date, 8);
				cout << fio << "ул." << street << "д." << base[i].ndom << " кв." << base[i].nkv << "\t" << date << endl;
			}*/
			}
		fbase.close();

		for (int i = 0; i < base.size(); i++) {
			addStreet(base[i], street, &h);
		}

		cout << "В базе " << streetCount(street) << " улиц." << endl;
		cout << endl;
		cout << "Список улиц находящихся в БД:" << endl;
		printStreet(street);
		system("pause");



		for (int i = 0; i < base.size(); i++) {
			fioTree(base[i], root, &h);
		}

		cout << "Построение АВЛ дерева завершено" << endl;
	}


	


	/*while (num != 0) {
		system("CLS");
		cout << "1.Задать размер массива для построения дерева \n";
		cout << "0. Выход\n";
		cin >> num;

		switch (num) {
		case 1:
			cout << "Введите размер массива для построения дерева" << endl;
			cin >> k;
			system("CLS");

			cout << "Размер массива: " << k << endl;
			M = new int[k];
			randomArray(M, k);

			cout << "Построение ДБ дерева" << endl;
			root = NULL;
			VR = true;
			HR = true;
			for (int i = 0; i < k; i++)
			{
				DBTree(M[i], &root);
			}
			printTree(root, 0);
			if (isSearchTree(root) == true) {
				cout << "Дерево является деревом поиска" << endl;
			}
			else {
				cout << "Дерево не является деревом поиска" << endl;
			}

			cout << endl;
			cout << "Размер дерева - " << treeSize(root) << endl;
			cout << "Высота дерева - " << treeHeight(root) << endl;
			cout << "Средняя высота дерева - " << averageHeight(root) << endl;
			cout << "Контрольная сумма дерева - " << treeCRC(root) << endl;
			cout << "Введите ключ для поиска :";
			cin >> key;
			searchRez = searchNode(root, key);
			if (searchRez != NULL) {
				cout << "Узел найден - " << searchRez->data << endl;
			}
			else {
				cout << "Узел не найден" << endl;
			}

			system("pause");

			system("CLS");


			cout << "Построение АВЛ дерева" << endl;
			printArray(M, k);
			root = NULL;
			for (int i = 0; i < k; i++)
			{
				avlTree(M[i], &root, &h);
			}

			printTree(root, 0);
			if (isSearchTree(root) == true) {
				cout << "Дерево является деревом поиска" << endl;
			}
			else {
				cout << "Дерево не является деревом поиска" << endl;
			}

			cout << endl;
			cout << "Размер дерева - " << treeSize(root) << endl;
			cout << "Высота дерева - " << treeHeight(root) << endl;
			cout << "Средняя высота дерева - " << averageHeight(root) << endl;
			cout << "Контрольная сумма дерева - " << treeCRC(root) << endl;
			cout << "Введите ключ для поиска :";
			cin >> key;
			searchRez = NULL;
			searchRez = searchNode(root, key);
			if (searchRez != NULL) {
				cout << "Узел найден - " << searchRez->data << endl;
			}
			else {
				cout << "Узел не найден" << endl;
			}
			system("pause");
			system("CLS");

			num = 3;

			delete[] M;
			break;
		default: break;
		}

	}*/

}

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	mMenu();

}







