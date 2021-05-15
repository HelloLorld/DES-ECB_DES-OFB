#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

const int IP1[8][8]
{
	{58,50,42,34,26,18,10,2},
	{60,52,44,36,28,20,12,4},
	{62,54,46,38,30,22,14,6},
	{64,56,48,40,32,24,16,8},
	{57,49,41,33,25,17,9,1},
	{59,51,43,35,27,19,11,3},
	{61,53,45,37,29,21,13,5},
	{63,55,47,39,31,23,15,7}
};

const int IP2[8][8]
{
	{40,8,48,16,56,24,64,32},
	{39,7,47,15,55,23,63,31},
	{38,6,46,14,54,22,62,30},
	{37,5,45,13,53,21,61,29},
	{36,4,44,12,52,20,60,28},
	{35,3,43,11,51,19,59,27},
	{34,2,42,10,50,18,58,26},
	{33,1,41,9,49,17,57,25}
};

const int E[8][6]
{
	{32,1,2,3,4,5},
	{4,5,6,7,8,9},
	{8,9,10,11,12,13},
	{12,13,14,15,16,17},
	{16,17,18,19,20,21},
	{20,21,22,23,24,25},
	{24,25,26,27,28,29},
	{28,29,30,31,32,1}
};


const int P[8][4]
{
	{16,7,20,21},
	{29,12,28,17},
	{1,15,23,26},
	{5,18,31,10},
	{2,8,24,14},
	{32,27,3,9},
	{19,13,30,6},
	{22,11,4,25},
};

const int G[8][7]
{
	{57,49,41,33,25,17,9},
	{1,58,50,42,34,26,18},
	{10,2,59,51,43,35,27},
	{19,11,3,60,52,44,36},
	{63,55,47,39,31,23,15},
	{7,62,54,46,38,30,22},
	{14,6,61,53,45,37,29},
	{21,13,5,28,20,12,4}
};

const int SHIFT[16]
{ 1,1,2,2,2,2,2,2,1,2,2,2,2,2,2,1 };

const int H[8][6]
{
	{14,17,11,24,1,5},
	{3,28,15,6,21,10},
	{23,19,12,4,26,8},
	{16,7,27,20,13,2},
	{41,52,31,37,47,55},
	{30,40,51,45,33,48},
	{44,49,39,56,34,53},
	{46,42,50,36,29,32}
};

std::vector<int> permutation(std::vector<int>, bool);	//Функция перестановки
void printbBit(int);
std::vector<int> expansion(std::vector<int> str);		//Функция расширения
std::vector<int> XOR(std::vector<int> str_first, std::vector<int> str_second);	//XOR функция
std::vector<int> s_box(std::vector<int> six_bit);		//Функция прохождения S матриц
std::vector<int> preparingKey(std::string key);			//Подготовка ключа начальная
std::vector<int> cyclic_permutation(std::vector<int>key, int J);	//Циклическая перестановка ключа под раунд
std::vector<int> DES(std::vector<int> encrypted, std::vector<int> key);	//Раунд шифрования DES
std::string encryption(std::string encrypted, std::string key);		//Функция шифрования
std::string decryption(std::string cipher, std::string key);		//Функция расшифрования


int main()
{
	std::string encrypted;
	std::getline(std::cin, encrypted);
	std::cout << "\n";
	std::string str;
	std::vector<int> vecStr;

	std::string key;
	std::cout << "\n";
	for (int i = 0; i < encrypted.length(); i++)
		std::cout << (int)encrypted[i] << "\t";
	std::cout << "\nEnter key: ";
	while (true)
	{
		std::cin >> key;
		if (sizeof(char) * key.length() * 8 == 56) break;
		std::cout << "Please enter key of 7 symbols!\n";
	}
	std::string cipher = encryption(encrypted, key);
	std::cout << "\nYour cipher:\n";
	for (int i = 0; i < cipher.length(); i++)
		std::cout << (int)cipher[i] << "\t";
	std::cout << "\n";
	for (int i = 0; i < cipher.length(); i++)
		std::cout << cipher[i];
	std::cout << "\n-------------------------------------------------------------------------\n";
	std::string original = decryption(cipher, key);
	std::cout << "\nYour original string:\n";
	for (int i = 0; i < original.length(); i++)
		std::cout << (int)original[i] << "\t";
	std::cout << "\n";
	for (int i = 0; i < original.length(); i++)
		std::cout << original[i];
	return 0;
}

std::string encryption(std::string encrypted, std::string key)
{
	std::string cipher;			//Строка под шифр
	std::vector<int>vecCipher;	//Вектор под шифр
	std::vector<int>vecEncrypted;
	//Запись строки в вектор
	for (int i = 0; i < encrypted.length(); i++)
	{
		int tmp = (unsigned char)encrypted[i];
		vecEncrypted.push_back(tmp);
	}

	if (vecEncrypted.size() % 8 != 0)
	{
		while (vecEncrypted.size() % 8 != 0)
		{
			vecEncrypted.push_back(180);
		}
	}

	std::vector<int>vecKey;		//Вектор ключа
	vecKey = preparingKey(key);	//Подготовка ключа
	std::vector<int>IVector;	//Вектор инициализации
	for (int i = 0; i < vecKey.size(); i++)
	{
		IVector.push_back(i ^ vecKey[i]);
	}

	//Запуск раундов шифрования
	if (vecEncrypted.size() / 8 == 1)
	{
		vecCipher = IVector;
		vecCipher = permutation(vecCipher, true);	//Начальная перестановка блока
		for (int i = 0; i < 16; i++)
		{
			std::vector<int>subKey = cyclic_permutation(vecKey, i);	//Получение подключа на раунд
			vecCipher = DES(vecCipher, subKey);						//Раунд шифрования
		}
		vecCipher = permutation(vecCipher, false);	//Конечная перестановка блока
			//Запись зашифрованного сообщения для возврата
		vecCipher = XOR(vecCipher, vecEncrypted);
		for (int i = 0; i < vecCipher.size(); i++)
		{
			cipher += (unsigned char)vecCipher[i];
		}
	}
	else
	{
		std::vector<int> blockEncr;
		for (int i = 0, counter = 0; i < vecEncrypted.size(); i++, counter++)
		{
			blockEncr.push_back(vecEncrypted[i]);
			if (counter == 7)
			{
				vecCipher = permutation(IVector, true);	//Начальная перестановка блока
				for (int i = 0; i < 16; i++)
				{
					std::vector<int>subKey = cyclic_permutation(vecKey, i);	//Получение подключа на раунд
					vecCipher = DES(vecCipher, subKey);						//Раунд шифрования
				}
				vecCipher = permutation(vecCipher, false);	//Конечная перестановка блока
				IVector = vecCipher;						//Вектор инициализации переходит на следующий блок
				vecCipher = XOR(vecCipher, blockEncr);		//XOR вектора инициализации и шифруемого сообщения
					//Запись зашифрованного сообщения для возврата
				for (int i = 0; i < vecCipher.size(); i++)
				{
					cipher += (unsigned char)vecCipher[i];
				}
				vecCipher.clear();
				blockEncr.clear();
				counter = -1;
			}
		}
	}

	return cipher;
}

std::string decryption(std::string cipher, std::string key)
{
	std::string original;		//Оригинальная строка
	std::vector<int>vecEcnrypted;	//Вектор под шифр
	std::vector<int>vecCipher;

	//Запись строки в вектор
	for (int i = 0; i < cipher.length(); i++)
	{
		int tmp = (unsigned char)cipher[i];
		vecEcnrypted.push_back(tmp);
	}

	std::vector<int>vecKey;		//Вектор ключа
	vecKey = preparingKey(key);	//Начальная подготовка ключа
	std::vector<int>IVector;	//Вектор инициализации
	for (int i = 0; i < vecKey.size(); i++)
	{
		IVector.push_back(i ^ vecKey[i]);
	}
	//Запуск обратного раунда шифрования
	std::vector<int>blockEncr;
	for (int i = 0, counter = 0; i < vecEcnrypted.size(); i++, counter++)
	{
		blockEncr.push_back(vecEcnrypted[i]);
		if (counter == 7)
		{
			vecCipher = permutation(IVector, true);	//Начальная перестановка блока
			for (int i = 0; i < 16; i++)
			{
				std::vector<int>subKey = cyclic_permutation(vecKey,i);	//Получение подключа на раунд
				vecCipher = DES(vecCipher, subKey);							//Раунд шифрования
			}

			vecCipher = permutation(vecCipher, false);	//Конечная перестановка
			IVector = vecCipher;						//Вектор инициализации переходит на следующий блок
			vecCipher = XOR(vecCipher, blockEncr);		//XOR вектора инициализации и зашифрованного сообщения
			//Запись расшифрованного сообщения
			for (int i = 0; i < vecCipher.size(); i++)
			{
				if (vecCipher[i] != 180)
					original += (unsigned char)vecCipher[i];
			}
			vecCipher.clear();
			blockEncr.clear();
			counter = -1;
		}
	}

	return original;
}

std::vector<int> cyclic_permutation(std::vector<int>key, int I)
{
	std::vector<int> permKey;	//Вектор для перестановки
	std::vector<int>leftHalf;	//Левая половина ключа
	std::vector<int>rightHalf;	//Правая половина ключа
	std::vector<int>bits;		//Вектор для бит

	//Разделение ключа на левую и правую части
	for (int i = 0; i < key.size(); i++)
	{
		if (i < key.size() / 2) leftHalf.push_back(key[i]);
		else rightHalf.push_back(key[i]);
	}

	//Циклическая перестановка под раунд
	int tmp = 0;
	for (int round = 0; round < I + 1; round++)
	{
		for (int j = 0; j < SHIFT[round]; j++)
		{
			for (int i = 0; i < 27; i++)
			{
				tmp = leftHalf[i];
				leftHalf[i] = leftHalf[i + 1];
				leftHalf[i + 1] = tmp;
				//------------------//
				tmp = rightHalf[i];
				rightHalf[i] = rightHalf[i + 1];
				rightHalf[i + 1] = tmp;
			}
		}
	}

	//Соединение левой и правой части
	for (int i = 0; i < 28; i++)
	{
		leftHalf.push_back(rightHalf[i]);
	}

	//Перестановка ключа через матрицу H
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 6; j++)
		{
			bits.push_back(leftHalf[H[i][j] - 1]);
		}

	tmp = 0;
	for (int i = 0; i < bits.size(); i += 8)
	{
		for (int j = 0; j < 8; j++)
		{
			tmp |= bits[i + j] << (7 - j);
		}
		permKey.push_back(tmp);
		tmp = 0;
	}

	return permKey;
}

std::vector<int> DES(std::vector<int> encrypted, std::vector<int> key)
{
	std::vector<int> resF;
	std::vector<int> bits;			//Вектор для битов
	std::vector<int> R;
	std::vector<int> L;

	//Разделение на левые и правые биты
	for (int i = 0; i < encrypted.size(); i++)
		if (i < encrypted.size() / 2) L.push_back(encrypted[i]);
		else R.push_back(encrypted[i]);

	std::vector<int>next;	//Перенос правой части на следующий раунд
	next = R;


	//---Функция f в DES алгоритме---//
		resF = expansion(R);						//Расширяем строку в соответсвии алгоритму	 
		resF = XOR(resF, key);						//Производим XOR младших битов и ключа

	//Записываем биты для дальнейшего разделения их на блоки по 6 бит
	for (int i = 0; i < resF.size(); i++)
	{
		for (int j = 0; j < 8; j++)
			if (resF[i] & (1 << (7 - j))) bits.push_back(1);
			else bits.push_back(0);
	}
	std::vector<int> six_bit;					//Вектор для 6 битных блоков
	int tmp = 0;

	//Запись 6 битного блока
	for (int i = 0; i < bits.size(); i += 6)
		for (int j = 0; j < 6; j++)
		{
			tmp |= bits[i + j] << (5 - j);
			if (j == 5)
			{
				six_bit.push_back(tmp);
				tmp = 0;
			}
		}

		R = s_box(six_bit);					//Прохождение 6 битных блоков через S боксы
	//-------------------------------//

		R = XOR(R, L);								//XOR левой части и правой, прошедшей через функцию


	//Соединение блоков в один

	for (int i = 0; i < R.size(); i++)
		next.push_back(R[i]);


	return next;

}

std::vector<int> s_box(std::vector<int> six_bit)
{
	int arr_S[8][4][16] = {// Массив S боксов
		{//S1
			{14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7},
			{0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8},
			{4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0},
			{15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13}
		},
		{//S2
			{15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10},
			{3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5},
			{0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15},
			{13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9}
		},
		{//S3
			{10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8},
			{13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1},
			{13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7},
			{1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12}
		},
		{//S4
			{7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15},
			{13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9},
			{10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4},
			{3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14}
		},
		{//S5
			{2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9},
			{14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6},
			{4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14},
			{11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3}
		},
		{//S6
			{12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11},
			{10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8},
			{9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6},
			{4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13}
		},
		{//S7
			{4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1},
			{13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6},
			{1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2},
			{6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12}
		},
		{//S8
			{13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7},
			{1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2},
			{7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8},
			{2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11}
		}
	};

	std::vector<int> four_bit;	//Вектор полученный после прохождения через S боксы
	int line = 0;				//Строка S бокса
	int column = 0;				//Столбец S бокса
	std::vector<int>bits;		//Вектор бит

	//Получение строки и столбца S бокса
	for (int i = 0; i < 8; i++)
	{
		//Получение строки S бокса
		if (six_bit[i] & (1 << 5)) line |= (1 << 1);
		else line |= (0 << 1);
		if (six_bit[i] & (1 << 0)) line |= (1 << 0);
		else line |= (0 << 0);
		//--------------------//

		//Получение столбца S бокса
		for (int j = 4; j >= 1; j--)
			if (six_bit[i] & (1 << j)) bits.push_back(1);
			else bits.push_back(0);
		for (int j = 0; j < bits.size(); j++)
			column |= bits[j] << (3 - j);
		//-----------------------------//

		four_bit.push_back(arr_S[i][line][column]);//Запись после прохождения S бокса
		bits.clear();
		column = 0;
		line = 0;
	}
	bits.clear();

	//Заполнение вектора битов (32 бит)
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if ((1 << (3 - j) & four_bit[i])) bits.push_back(1);
			else bits.push_back(0);
		}
	}

	four_bit.clear();//Очищаем четырехбитные блоки, для записи в них перестановки битов

	//Запись перестановки бит через матрицу P
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 4; j++)
			four_bit.push_back(bits[P[i][j] - 1]);

	std::vector<int> strFromP;	//Строка после перестановки P
	int tmp = 0;

	//Запись в строку (замениь на другой перевод в десятичную!!!!!)
	for (int i = 0; i < four_bit.size(); i += 8)
	{
		for (int j = 0; j < 8; j++)
		{
			tmp |= four_bit[i + j] << (7 - j);
		}
		strFromP.push_back(tmp);
		tmp = 0;
	}
	return strFromP;
}

std::vector<int> preparingKey(std::string key)
{
	std::vector<int>bits;	//Вектор бит
	std::vector<int>::iterator position;

	//Перевод ключа в двоичную систему
	for (int i = 0; i < key.length(); i++)
	{
		for (int j = 0; j < 8; j++)
			if (key[i] & (1 << (7 - j))) bits.push_back(1);
			else bits.push_back(0);
	}

	//Добавление битов четности
	std::vector<int>chet;
	for (int i = 0, counter = 0, j = 1; i < bits.size(); i++, j++)
	{
		if (bits[i] == 1) counter++;
		chet.push_back(bits[i]);
		if (j % 7 == 0)
		{
			if ((counter % 2 == 0) && (i != 0)) chet.push_back(1);
			else chet.push_back(0);
			counter = 0;
		}
	}
	std::vector<int>keyGtable;	//Ключ после матрицы G

	//Перестановка ключа по таблице G
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 7; j++)
		{
			keyGtable.push_back(chet[G[i][j] - 1]);
		}

	return keyGtable;//Ключ в виде бит
}

std::vector<int> permutation(std::vector<int> str, bool mode)
{//Функция перестановки битов через матрицы перестановок	
	std::vector<int> permStr;//Строка после перестановки битов
	int tmp = 0;
	std::vector<int> bits;//Вектор для битов

	//Перевод блока в двоичную систему
	for (int i = 0; i < str.size(); i++)
	{
		for (int j = 0; j < 8; j++)
			if (str[i] & (1 << (7 - j))) bits.push_back(1);
			else bits.push_back(0);
	}

	//Перестановка блока через матрицу
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{// true-матрица начальной перестановки; false-матрица конечной перестановки
			if (mode) permStr.push_back(bits[IP1[i][j] - 1]);
			else permStr.push_back(bits[IP2[i][j] - 1]);
		}
	}

	bits.clear();

	for (int i = 0; i < permStr.size(); i += 8)
	{
		for (int j = 0; j < 8; j++)
		{
			tmp |= permStr[i + j] << (7 - j);
		}
		bits.push_back(tmp);
		tmp = 0;
	}

	return bits;
}


std::vector<int> expansion(std::vector<int> str)
{//Функция расширения через матрицу E 
	std::vector<int> expansionStr;//Расширенная строка в виде бит
	int tmp = 0;
	std::vector<int> bits;//Вектора для битов

	//перевод в двоичную систему
	for (int i = 0; i < str.size(); i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (str[i] & (1 << (7 - j))) bits.push_back(1);		//Записываем биты 
			else bits.push_back(0);
		}
	}

	//Расширение через матрицу E
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			expansionStr.push_back(bits[E[i][j] - 1]);
		}
	}
	bits.clear();

	for (int i = 0; i < expansionStr.size(); i += 8)
	{
		for (int j = 0; j < 8; j++)
		{
			tmp |= expansionStr[i + j] << (7 - j);
		}
		bits.push_back(tmp);
		tmp = 0;
	}

	return bits; //Строка расширенная в виде цифр
}

std::vector<int> XOR(std::vector<int> str_first, std::vector<int> str_second)
{
	std::vector<int> stringXOR;
	for (int i = 0, j = 0; i < str_first.size(); i++, j++)
	{
		if (!(j < str_second.size() + 1)) j = 0;
		int first = str_first[i];
		int second = str_second[j];
		int a = first ^ second;
		stringXOR.push_back(a);
	}
	return stringXOR;
}

void printbBit(int a)
{
	for (int i = 7; i >= 0; i--)
		if (a & (1 << i)) std::cout << "1";
		else std::cout << "0";
}
