#include <Windows.h>
#include <vector>
#include <string>
#include <map>

#include "framework.h"
using namespace std;

class Transaction {// базовый класс транзакции
public:
	virtual ~Transaction(){} // виртуальный деструктор
	virtual double getAmound() const = 0; // возвращает сумму транзакции
	virtual wstring getCategory() const = 0; // возвращает категорию транзакции

};

// Интерфейс для управления категориями трат
class CategoryManagerInterface {
public:
	virtual ~CategoryManagerInterface(){}// виртуальный деструктор
	virtual void addCategory(const wstring& category) = 0; // добавляет категорию
	virtual void removeCatogory(const wstring& category) = 0; // удаляет категорию
	virtual vector<wstring> getAllCategories() const = 0;// возвращает все категории
};

// Интерфейс для отображения графической диаграммы расходов
class ChartRendererInterface {
public:
	virtual ~ChartRendererInterface(){}// виртуальный деструктор
	virtual void renderChart(map<wstring, double>& data) = 0;// отображает график
};

// Интерфейс для сохранения и загрузки данных о транзакциях и заработке
class DataStorageInterface {
public:
	virtual ~DataStorageInterface(){}// виртуальный деструктор
	virtual void saveData(const map<wstring, double>& data);// сохраняет данные
	virtual map<wstring, double> loadData() = 0;// загружает данные
};

// Интерфейс для взаимодействия с пользователем
class UserInputInterface {
public:
	virtual ~UserInputInterface(){}// виртуальный деструктор
	virtual void displayInputDialoge(const vector<wstring>& category) = 0;// отображает диалоговое окно
	virtual void displayManager(const wstring& message) = 0;// отображает менеджер
};
// Интерфейс для вычисления баланса и доступных средств
class BalanceCalculatorInterface {
public:
	virtual ~BalanceCalculatorInterface() {} // Виртуальный деструктор для правильного удаления объектов
	virtual double calculateBalance(const std::map<std::wstring, double>& expenses, double income) const = 0;// вычисляет баланс
	virtual double calculateAvailableFunds(double balance, double goal) const = 0; 
};

// Интерфейс для управления целями сбережений
class SavingsGoalManagerInterface {
public:
	virtual ~SavingsGoalManagerInterface() {} // Виртуальный деструктор для правильного удаления объектов
	virtual void setSavingsGoal(double goal) = 0; 
	virtual double getSavingsGoal() const = 0; 
};

// Интерфейс для просмотра данных предыдущих месяцев
class PreviousMonthsViewerInterface {
public:
	virtual ~PreviousMonthsViewerInterface() {} // Виртуальный деструктор для правильного удаления объектов
	virtual void showPreviousMonthData(const std::map<std::wstring, double>& expenses) = 0; 
};

// Финансовый трекер приложение
class FinanceTracker: public CategoryManagerInterface, public ChartRendererInterface, 
	public DataStorageInterface, public UserInputInterface, public BalanceCalculatorInterface,
	public SavingsGoalManagerInterface, public PreviousMonthsViewerInterface {
private:
	map<wstring, double> expense; // расходы
	double totalIncome; // общий доход
	double savingGoals; // цель сбережений
	vector<wstring> categories; // категории
	HWND hWnd; // дескриптор окна
