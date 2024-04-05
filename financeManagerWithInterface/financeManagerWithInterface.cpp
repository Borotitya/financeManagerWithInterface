#include<Windows.h> // библиотека для работы с окнами
#include <iostream> // библиотека для работы с потоками ввода/вывода
#include<string> // библиотека для работы со строками
#include<vector> // библиотека для работы с векторами
#include<sstream> // библиотека для работы со строковыми потоками
#include<iomanip> // библиотека для работы с форматированием вывода

using namespace std; // использование пространства имен std
class InterfaceTransaction { // интерфейс транзакции
public: // публичный интерфейс
	virtual ~InterfaceTransaction() {} // виртуальный деструктор
	virtual wstring getCategory() const = 0; // возвращает категорию транзакции
	virtual double getAmount() const = 0; // возвращает сумму транзакции
};

class InterfaceFinanceTracker { // интерфейс финансового трекера
public: 
	virtual ~InterfaceFinanceTracker() {} // виртуальный деструктор
	virtual void setIncome(double newIncome) = 0; // устанавливает доход
	virtual void addTransaction(const wstring& category, double amount) = 0; // добавляет транзакцию
	virtual double calculateTotal() const = 0; // возвращает общую сумму транзакций
	virtual vector<InterfaceTransaction*> getTransactions() const = 0; // возвращает вектор транзакций
	virtual double getIncome() const = 0; // метод, возвращающий полученный доход
};

class InterfaceTransactionCategory {// интерфейс категории транзакции
public: // публичный интерфейс
	virtual ~InterfaceTransactionCategory() {} // виртуальный деструктор
	virtual void addCategory(const wstring& category) = 0; // добавляет категорию
	virtual vector<wstring> getCategories() const = 0; // возвращает вектор категорий
	virtual void showNotification(const wstring& message) const = 0; // показывает уведомление
};

class InterfaceNotification { // интерфейс уведомления
public: // публичный интерфейс
	virtual ~InterfaceNotification() {} // виртуальный деструктор
	virtual void showNotification(const wstring& message) const = 0; // показывает уведомление
};


	class FinanceTracker : public InterfaceFinanceTracker, public InterfaceTransactionCategory, public InterfaceNotification { // класс финансового трекера
	private: // приватные поля
		double income; // доход
		vector<InterfaceTransaction*> transactions; // вектор транзакций
		vector<wstring> categories; // Добавляем вектор для хранения категорий
	public: // публичный интерфейс
		FinanceTracker() : income(0) {} // конструктор по умолчанию
		// Реализация методы IFinanceTracker
		void setIncome(double newIncome) override { // устанавливает доход
			income = newIncome;
		}
		void addTransaction(const wstring& category, double amount) override { // добавляет транзакцию
			if ((category == L"Общие" || category == L"Одиночные") && amount > income) { // если категория "Общие" или "Одиночные" и сумма больше дохода
				showNotification(L"Сумма транзакции не может превышать доход!!"); // показываем уведомление
				return; // выходим из метода
			}
			else if (category != L"Общие" && category != L"Одиночные" && calculateTotal() + amount > income) { // если категория не "Общие" и не "Одиночные" и общая сумма транзакций больше дохода
				showNotification(L"Сумма транзакции не может превышать доход!!"); // показываем уведомление
				return; // выходим из метода
			}
			transactions.push_back(new Transaction(category, amount)); // добавляем транзакцию в вектор
		}
		double calculateTotal() const override {
			double total = 0; // общая сумма
			for (auto transaction : transactions) { // для каждой транзакции
				total += transaction->getAmount(); // добавляем сумму транзакции к общей сумме
			}
			return total; // возвращаем общую сумму
		}
		vector<InterfaceTransaction*> getTransactions() const override { // возвращает вектор транзакций
			return transactions; // возвращаем вектор транзакций
		}
		double getIncome() const override { // метод, возвращающий полученный доход
			return income; // возвращаем доход
		}
		vector <wstring>getCategories() const override { // метод, возвращающий категории
			return categories; // возвращаем категории
		}
		void showNotification(const wstring& message) const override { // показывает уведомление
			MessageBox(NULL, message.c_str(), L"Уведомление", MB_OK | MB_ICONINFORMATION); // показываем уведомление
		}
	private:
		class Transaction : public InterfaceTransaction {
		private:
			std::wstring category;
			double amount;
		public:
			Transaction(const std::wstring& category, double amount) : category(category), amount(amount) {}

			std::wstring getCategory() const override {
				return category;
			}

			double getAmount() const override {
				return amount;
			}
		};
	};
// Функция для обновления общей суммы потраченных средств и списка транзакций
	void UpdateTotalSpent(InterfaceFinanceTracker* tracker, HWND hWnd) { 
		// Получаем общую сумму потраченных средств
		double totalSpent = tracker->calculateTotal();

		// Формируем строку для отображения общей суммы
		wostringstream totalSpentStream;
		totalSpentStream << L"Общая сумма потраченных средств : $" <<fixed << setprecision(2)<< totalSpent; // формируем строку для отображения общей суммы

		// Находим элемент управления STATIC для общей суммы
		HWND hStaticTotalSpent = GetDlgItem(hWnd, 7);
		// Устанавливаем текст элемента управления STATIC для общей суммы
		SetWindowText(hStaticTotalSpent, totalSpentStream.str().c_str());

		// Получаем текущий доход
		double income = tracker->getIncome();

		// Формируем строку для отображения текущего дохода
		wostringstream incomeStream;
		incomeStream << L"Текущий доход : $" << fixed << setprecision(2) << income; // формируем строку для отображения текущего дохода

		// Находим элемент управления STATIC для текущего дохода
		HWND hStaticIncome = GetDlgItem(hWnd, 9);
		// Устанавливаем текст элемента управления STATIC для текущего дохода
		SetWindowTextW(hStaticIncome, incomeStream.str().c_str());

		// Обновляем список транзакций
		HWND hListTransactions = GetDlgItem(hWnd, 8);
		// Очищаем список
		SendMessage(hListTransactions, LB_RESETCONTENT, 0, 0);

		// Получаем список транзакций
		vector<InterfaceTransaction*> transactions = tracker->getTransactions();
		// Добавляем каждую транзакцию в список
		for (const auto& transaction : transactions) {
			// Формируем строку для отображения категории и суммы транзакции
			wostringstream transactionStream;
			transactionStream << L"Категория : " << transaction->getCategory() << L", Сумма: $" << fixed << setprecision(2) << transaction->getAmount();// формируем строку для отображения категории и суммы транзакции
			// Добавляем строку в список
			SendMessage(hListTransactions, LB_ADDSTRING, 0, (LPARAM)transactionStream.str().c_str());
		}
	}
	// Функция для обновления списка категорий транзакций в выпадающем списке ComboBox
	LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {

}
   

