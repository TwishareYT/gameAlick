#include <iostream>
#include <string>
#include <conio.h>
#include "windows.h"
#include <fstream>
#include <vector>
using namespace std;

bool map_change{ true };
bool gameOver{ false }; //признак законченности игры
ifstream current_map; // файл текущей карты 
string current_map_name = "_start_location.txt";
vector<string> current_draw_map; // текущая карта на отрисовку
int width{ 0 }, height{ 0 };// ширина длина карты

enum move_var { STOP = 0, LEFT, RIGHT, UP, DOWN }; // перечисление возможных перемещений
enum ch_race { HUMAN = 0, ELF, DWARF }; // вариант расы
enum ch_gender { MALE = 0, FEMALE }; // пол персонажа (м, ж)
enum ch_class { WIZARD = 0, ASSASIN, PRIST, WARRIOR, NECROMANCER }; // вариант класса персонажа (маг, вор, жрец, воин, некромант)
enum ch_armour { DRESS = 0, LEATHER, CHAIN, PLATE, NAKED }; // броня (тряпки, кожа, кольчуга, латы, голый)
enum ch_weapon {
	DAGGER = 0, DDAGGER, SWORD, DSWORD, AXE, DAXE, MACE, THSWORD, THAXE, BOW, SHIELD,
	GRIMUAR, MAGIC_STAFF, MAGIC_STICK, MAGIC_SCROLL, NONWEAPON
}; // оружие (кинжал, два кинажал, меч, два меча, 
		  //топор, два топора, булава, двуручный меч, 
		  //двуручный топор, лук, щит, гримуар, волшебный посох, 
		  //волшебная палочка, свиток заклинаний, безоружный)

struct  Character
{
	char ch_skin{ 'T' }; //внешний вид персонажа
	string _name{ "" }; //имя персонажа
	ch_race _race{ HUMAN }; //раса
	ch_gender _gender{ MALE }; //пол
	ch_class _class{ WIZARD }; //класс
	ch_weapon _weapon{ NONWEAPON }; //оружие
	ch_armour _armour{ NAKED }; //броня
	string _autobiography{ "" }; //предыстория
	double _coins{ 0.0 }; //деньги
	double _heals{ 100.0 }; //количество здоровья
	double _mana{ 100.0 }; //количество маны(выносливости)
	int ch_x{ 0 }; //вниз
	int ch_y{ 0 }; //вправо
	move_var ch_move;
};
Character character;

void Setup(); //функция подгрузки стартовых параметров игры (запускается один раз при старте)
void DrawMap(char skin, string map_name, move_var ch_move); // функция отрисовки карты (вызывается каждый раз при перемещении персонажа)
//принимает на вход: skin -  символ отображения персонажа, map_name - название файла карты ("С:\...\location.txt"), ch_move - направление 
//движения персонажа из перечисления move_var (STOP, UP, LEFT, RIGHT, DOWN)
void Input(); // функция обработки пользовательского ввода
void Logic(); // логика игры

int main()
{
	Setup(); //подгружаем стартовые параметры игры
	while (!gameOver) //пока игра не окончена
	{
		Input(); //обрабатываем пользовательский ввод
		Logic(); //отрабатываем логику игры
	}
	if (gameOver) //если игра окночена (gameOver == true)
	{
		system("cls"); // чистим экран
		cout << "Game over!"; // выводим надпись Game over!
	}
	return 0;
}
void Setup()
{
	/*чистим экран, вводим имя, выбираем расу, класс, ... , в зависимости от выбора оператором switch case выводим соответсвующую
	информацию на экран. В случае с полом, кроме сообщения на экран, определяем симовол отображения персонажа*/
	system("cls");
	int temp{ 0 };
	cout << "Character editor" << endl;
	// имя
	cout << "Input the name of your character: " << endl;
	getline(cin, character._name);
	cout << "The name of your character " << character._name << endl;
	//раса
	cout << "Select the race of your character: \n    0 - Human\n    1 - Elf\n    2 - Dwarf" << endl;
	cin >> temp;
	switch (temp)
	{
	case 0: character._race = HUMAN; cout << "Character race is Human" << endl; break;
	case 1: character._race = ELF; cout << "Character race is Elf" << endl; break;
	case 2: character._race = DWARF; cout << "Character race is Dwarf" << endl; break;
	default: character._race = HUMAN; cout << "Character race is Human" << endl; break;
	}
	//пол
	cout << "Select the gender of your character: \n    0 - Male\n    1 - Female" << endl;
	cin >> temp;
	switch (temp)
	{
	default: character._gender = MALE; cout << "The gender of your character male" << endl; character.ch_skin = 'T'; break; // мужской - Т, женский - Y
	case 0: character._gender = MALE; cout << "The gender of your character male" << endl; character.ch_skin = 'T'; break;
	case 1: character._gender = FEMALE; cout << "The gender of your character female" << endl; character.ch_skin = 'Y'; break;
	}
	//класс
	cout << "Select the class of your character: \n    0 - Wizard\n    1 - Assasin\n    2 - Prist\n    3 - Warrior\n    4 - Necromancer" << endl;
	cin >> temp;
	switch (temp)
	{
	default: character._class = WIZARD; cout << "The class of your character Wizard" << endl; break;
	case 0: character._class = WIZARD; cout << "The class of your character Wizard" << endl; break;
	case 1: character._class = ASSASIN; cout << "The class of your character Assasin" << endl; break;
	case 2: character._class = PRIST; cout << "The class of your character Prist" << endl; break;
	case 3: character._class = WARRIOR; cout << "The class of your character Warrior" << endl; break;
	case 4: character._class = NECROMANCER; cout << "The class of your character Necromancer" << endl; break;
	}
	//вводим биографию
	cout << "Biography of your character: " << endl;
	getline(cin, character._autobiography, ';'); //из потока cin (т.е потока ввода) получаем строку, пока не встретим символ-разделитель (делиметр)
	//и записываем эту строку в поле _autobography структуры Character
	cout << "Biography:" << character._autobiography << endl;
	character.ch_move = STOP; //на старт игры не движимся 
	character.ch_x = 1; // стоим в точке с координатами (2,2). ch_x - номер строки, ch_y - номер элемента в строке, порядковый номер которой ch_x
	character.ch_y = 1;
	DrawMap(character.ch_skin, current_map_name, character.ch_move); //отрисовываем карту
	//cout << "Character: " << character._name << " Race: " << character._race << " Class: " <<character._class<< " Gender: " << character._gender << endl;
}

void DrawMap(char skin, string map_name, move_var ch_move)
{
	system("cls"); //читсим экран
	string str{ "" };
	if (map_change)//если на карте произошли изменения (сдвинулся персонаж)
	{
		current_map.open(map_name);//открываем файл с картой
		if (!current_map.is_open())//проверяем открыт ли файл
		{//если не котрыт - выводим сообщение об ошибке
			cout << "Error. Map file " << map_name << " not open." << endl;
			return; //выодим из функции
		}
		while (getline(current_map, str))// пока из потока current_map (потока ввода из файла (ifstream - input file stream))
			//получаем строку (на этот раз без делиметра), записываем ее в переменную строкового типа str
		{
			current_draw_map.push_back(str); //в вектор строк current_draw_map в последний элемент (каждый элемент этого вектора - строка)
			//записваем строку str. Цикл повторяется, пока в файле не кончатся строки. В итоге весь файл запишется в вектор строк (построчно).
			//cout << current_draw_map.back()<<endl;
		}
		map_change = false; //возвращаем значение false флагу изменения карты. (чтобы отрисовывать карту только тогда, когда что то на нец изменилось)
		height = current_draw_map.size(); //167 Х. Высота - количество элементов в векторе строк (т.е количество строк в файле)
		width = current_draw_map.back().size();//38 Y. Ширина - количество символов в каждой строке.
	}
	//сама отрисовка карты. Пробегаем вектор строк поэлементно
	for (int i = 0; i < height; ++i)
	{
		//каждый элемент этого вектора(строку) так же пробегаем поэлементно
		for (int j = 0; j < width; ++j)
		{
			//находим координаты нашего персонажа (на старт игры - 2 строка, 2 элемент этой строки)
			if ((i == character.ch_x) && (j == character.ch_y))
			{

			}
		}
		//отрисовывем в консоль текущую строку вектора
		cout << current_draw_map[i] << endl;
	}
	//закрываем файл карты (обязательно! иначен ошибка)
	current_map.close();
	//под карту пишем состояние персонажа
	cout << "Character: " << character._name << " Race: " << character._race << " Class: " << character._class << " Gender: " << character._gender << endl;
	cout << "Coordinates: " << character.ch_x << "	" << character.ch_y << endl;
}

void Input()
{

}

void Logic()
{

}