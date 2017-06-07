#pragma once
#include "Lib.h"
class ThreadAffinity
{
	//��������� ��� �������� ������. -1 ���� ��������� �� ������ 
	static thread_local int		core;
	//���� ��� ������� ����� ������� ��� ���������� ������
	mutable std::atomic<unsigned short>	counter;
	//���-�� ���� � �������� ����������
	const unsigned short	total_cores;
	// ������������� ������ ���� ���
	// ���������� ����� ���� �� ������� �������� �����
	// -1 � ������ ������
	int set_cpu(const unsigned short& core) const;
public:
	ThreadAffinity();
	~ThreadAffinity();

	int set_core() const;
	// ���������� ���� ���������� ��� �������� ������.
	// -1 ���� ���� �� �������
	int get_core_num() const;

	// ���������� ����� ���������� ���� ���������� � ��������� �������. �� 1 � �����.
	int get_total_cores() const;
};

