#pragma once
#include "Lib.h"
class ThreadAffinity
{
	//процессор для текущего потока. -1 если процессор не выбран 
	static thread_local int		core;
	//Ядро для которое будет выбрано для следующего потока
	mutable std::atomic<unsigned short>	counter;
	//Кол-во ядер у текущего процессора
	const unsigned short	total_cores;
	// Устонавливаем потоку ядро ЦПУ
	// возвращаем номер ядра на котором работает поток
	// -1 в случае ошибки
	int set_cpu(const unsigned short& core) const;
public:
	ThreadAffinity();
	~ThreadAffinity();

	int set_core() const;
	// Возвращает ядро процессора для текущего потока.
	// -1 если ядро не выбрано
	int get_core_num() const;

	// Возвращает общее количество ядер процессора в локальной системе. От 1 и более.
	int get_total_cores() const;
};

