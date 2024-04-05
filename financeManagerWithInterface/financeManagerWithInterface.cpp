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


class FinanceTracker : public InterfaceFinanceTracker, public InterfaceTransactionCategory, public InterfaceNotification { // финансовый трекер с уведомлениями
private: // приватные поля
	double income; // доход
	vector<InterfaceTransaction*> transactions; // вектор транзакции
	vector<wstring> categories;  // вектор категорий
public:
	FinanceTracker() : income(0) {} // конструктор
	void setIncome(double newIncome) override { // устанавливает доход
		income = newIncome; // устанавливаем доход
	}

	void addTransaction(const wstring& category, double amount) override { // добавляет транзакцию
		if ((category == L"Общие" || category == L"Одиночные") && amount > income) { // если категория - общие или одиночные и сумма больше дохода
			showNotification(L"Сумма транзакции не может превышать доход!"); // показываем уведомление
			return; // выходим из функции
		}
		else if (category != L"Общие" && category != L"Одиночные" && calculateTotal() + amount > income) { // если категория не общие и не одиночные и сумма больше дохода
			showNotification(L"Сумма транзакции не может превышать доход!"); // показываем уведомление
			return;
		}
		transactions.push_back(new Transaction(category, amount)); // добавляем транзакцию
	}

	double calculateTotal() const override { // возвращает общую сумму транзакций
		double totalSpent = 0; // общая сумма потраченных средств
		for (const auto& transaction : transactions) { // для каждой транзакции
			totalSpent += transaction->getAmount(); // добавляем сумму транзакции к общей сумме
		}
		return totalSpent; // возвращаем общую сумму
	}

	vector<InterfaceTransaction*> getTransactions() const override { // возвращает вектор транзакций
		return transactions; // возвращаем вектор транзакций
	}

	double getIncome() const override { // метод, возвращающий полученный доход
		return income; // возвращаем доход
	}

	void addCategory(const wstring& category) override { // добавляет категорию
		categories.push_back(category); // добавляем категорию
	}

	vector<wstring> getCategories() const override {	// возвращает вектор категорий
		return categories; // возвращаем вектор категорий
	}

	void showNotification(const wstring& message) const override { // показывает уведомление
		MessageBoxW(NULL, message.c_str(), L"Уведомление", MB_OK | MB_ICONINFORMATION); // показываем уведомление сообщением в окне приложения 
	}

private: // приватные методы и поля класса FinanceTracker
	class Transaction : public InterfaceTransaction { // транзакция 
	private: 
		wstring category; // категория транзакции
		double amount; // сумма транзакции 
	public:
		Transaction(const wstring& category, double amount) : category(category), amount(amount) {} // конструктор транзакции с параметрами категории и суммы

		wstring getCategory() const override { // возвращает категорию транзакции
			return category;
		}

		double getAmount() const override { // возвращает сумму транзакции 
			return amount;
		}
	};
};
// Функция для обновления общей суммы потраченных средств и списка транзакций
void UpdateTotal(InterfaceFinanceTracker* tracker, HWND hWnd) {
    // Получаем общую сумму потраченных средств
    double totalSpent = tracker->calculateTotal(); // получаем общую сумму потраченных средств

    // Формируем строку для отображения общей суммы
    std::wostringstream totalSpentStr; 
    totalSpentStr << L"Общая сумма потраченных средств: $" << std::fixed << std::setprecision(2) << totalSpent; // формируем строку

    // Находим элемент управления STATIC для общей суммы
    HWND hStaticTotalSpent = GetDlgItem(hWnd, 7);
    // Устанавливаем текст элемента управления STATIC для общей суммы
    SetWindowTextW(hStaticTotalSpent, totalSpentStr.str().c_str());

    // Получаем текущий доход
    double income = tracker->getIncome();

    // Формируем строку для отображения текущего дохода
    std::wostringstream incomeStr;
    incomeStr << L"Текущий доход: $" << std::fixed << std::setprecision(2) << income; // формируем строку

    // Находим элемент управления STATIC для текущего дохода
    HWND hStaticIncome = GetDlgItem(hWnd, 9);
    // Устанавливаем текст элемента управления STATIC для текущего дохода
    SetWindowTextW(hStaticIncome, incomeStr.str().c_str());

    // Обновляем список транзакций
    HWND hList = GetDlgItem(hWnd, 8);
    // Очищаем список
    SendMessageW(hList, LB_RESETCONTENT, 0, 0);

    // Получаем список транзакций
    std::vector<InterfaceTransaction*> transactions = tracker->getTransactions();
    // Добавляем каждую транзакцию в список
    for (const auto& transaction : transactions) {
        // Формируем строку для отображения категории и суммы транзакции
        std::wostringstream transactionStr;
        transactionStr << L"Категория: " << transaction->getCategory() << L", Сумма: $" << std::fixed << std::setprecision(2) << transaction->getAmount();
        // Добавляем строку в список
        SendMessageW(hList, LB_ADDSTRING, 0, (LPARAM)transactionStr.str().c_str());
    }
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) { // обработчик сообщений окна
    static InterfaceFinanceTracker* tracker = new FinanceTracker(); // создаем объект трекера финансов

    switch (message) { // обработка сообщений
    case WM_CREATE: // создание окна
    {
        // Создание элементов управления
        CreateWindowW(L"STATIC", L"Доход:", WS_VISIBLE | WS_CHILD, 10, 10, 80, 30, hWnd, NULL, NULL, NULL); // создаем элемент управления STATIC для текста "Доход"
        CreateWindowW(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 100, 10, 100, 30, hWnd, (HMENU)1, NULL, NULL); // создаем элемент управления EDIT для ввода
        CreateWindowW(L"BUTTON", L"Установить доход", WS_VISIBLE | WS_CHILD, 210, 10, 150, 30, hWnd, (HMENU)2, NULL, NULL); // создаем элемент управления BUTTON для установки дохода
        CreateWindowW(L"STATIC", L"Категория:", WS_VISIBLE | WS_CHILD, 10, 50, 80, 30, hWnd, NULL, NULL, NULL); // создаем элемент управления STATIC для текста "Категория"
        HWND hCombo = CreateWindowW(L"COMBOBOX", L"", WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST, 100, 50, 150, 200, hWnd, (HMENU)3, NULL, NULL); // создаем элемент управления COMBOBOX для выбора категории
        SendMessageW(hCombo, CB_ADDSTRING, 0, (LPARAM)L"Общие"); // добавляем строку в элемент управления COMBOBOX
        SendMessageW(hCombo, CB_ADDSTRING, 0, (LPARAM)L"Одиночные"); // добавляем строку в элемент управления COMBOBOX
        SendMessageW(hCombo, CB_ADDSTRING, 0, (LPARAM)L"Развлечения"); // добавляем строку в элемент управления COMBOBOX
        SendMessageW(hCombo, CB_ADDSTRING, 0, (LPARAM)L"Еда"); // добавляем строку в элемент управления COMBOBOX
        SendMessageW(hCombo, CB_ADDSTRING, 0, (LPARAM)L"Транспорт"); // добавляем строку в элемент управления COMBOBOX
        SendMessageW(hCombo, CB_ADDSTRING, 0, (LPARAM)L"Одежда"); // добавляем строку в элемент управления COMBOBOX
        SendMessageW(hCombo, CB_ADDSTRING, 0, (LPARAM)L"Жилье"); // добавляем строку в элемент управления COMBOBOX
        SendMessageW(hCombo, CB_ADDSTRING, 0, (LPARAM)L"Здоровье"); // добавляем строку в элемент управления COMBOBOX
        SendMessageW(hCombo, CB_ADDSTRING, 0, (LPARAM)L"Образование"); // добавляем строку в элемент управления COMBOBOX
        SendMessageW(hCombo, CB_ADDSTRING, 0, (LPARAM)L"Прочее"); // добавляем строку в элемент управления COMBOBOX

        CreateWindowW(L"STATIC", L"Сумма:", WS_VISIBLE | WS_CHILD, 10, 90, 80, 30, hWnd, NULL, NULL, NULL); // создаем элемент управления STATIC для текста "Сумма"
        CreateWindowW(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 100, 90, 100, 30, hWnd, (HMENU)4, NULL, NULL); // создаем элемент управления EDIT для ввода
        CreateWindowW(L"BUTTON", L"Добавить транзакцию", WS_VISIBLE | WS_CHILD, 210, 90, 150, 30, hWnd, (HMENU)5, NULL, NULL); // создаем элемент управления BUTTON для добавления транзакции
        CreateWindowW(L"BUTTON", L"Обновить общую сумму", WS_VISIBLE | WS_CHILD, 10, 130, 150, 30, hWnd, (HMENU)6, NULL, NULL); // создаем элемент управления BUTTON для обновления общей суммы
        // Добавляем элемент управления STATIC для вывода общей суммы
        CreateWindowW(L"STATIC", L"", WS_VISIBLE | WS_CHILD, 10, 170, 350, 30, hWnd, (HMENU)7, NULL, NULL);
        // Добавляем элемент управления ListBox для отображения списка транзакций
        CreateWindowW(L"LISTBOX", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | WS_VSCROLL | LBS_NOTIFY, 10, 210, 380, 200, hWnd, (HMENU)8, NULL, NULL);
        // Добавляем элемент управления STATIC для вывода текущего дохода
        CreateWindowW(L"STATIC", L"", WS_VISIBLE | WS_CHILD, 10, 420, 350, 30, hWnd, (HMENU)9, NULL, NULL);
    }
    break;
    case WM_COMMAND: // обработка команд
    {
        int wmId = LOWORD(wParam); // получаем идентификатор команды 
        switch (wmId) 
        {
        case 2: // Установить доход
        {
            // Получаем значение введенного дохода
            wchar_t buffer[256];
            GetDlgItemTextW(hWnd, 1, buffer, 256); 
            double income = _wtof(buffer); // преобразуем строку в число

            // Устанавливаем доход в трекере финансов
            tracker->setIncome(income);

            MessageBoxW(hWnd, L"Доход успешно установлен!", L"Уведомление", MB_OK | MB_ICONINFORMATION); // показываем уведомление
        }
        break;
        case 5: // Добавить транзакцию
        {
            // Получаем выбранную категорию и сумму транзакции
            wchar_t categoryBuffer[256];
            GetDlgItemTextW(hWnd, 3, categoryBuffer, 256); 
            std::wstring category = categoryBuffer; // получаем категорию

            wchar_t amountBuffer[256]; // получаем сумму транзакции
            GetDlgItemTextW(hWnd, 4, amountBuffer, 256); // получаем сумму транзакции
            double amount = _wtof(amountBuffer); // преобразуем строку в число

            // Добавляем транзакцию в трекер финансов
            tracker->addTransaction(category, amount);

            MessageBoxW(hWnd, L"Транзакция успешно добавлена!", L"Уведомление", MB_OK | MB_ICONINFORMATION); // показываем уведомление
        }
        break;
        case 6: // Обновить общую сумму
            UpdateTotal(tracker, hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam); // обработка сообщения по умолчанию
        }
    }
    break;
    case WM_DESTROY: // уничтожение окна 
        PostQuitMessage(0); // посылаем сообщение о завершении работы приложения
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam); // обработка сообщения по умолчанию
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) { // точка входа в программу
    // Определение класса окна 
    WNDCLASSEXW wcex;
    wcex.cbSize = sizeof(WNDCLASSEX); // размер структуры
    wcex.style = CS_HREDRAW | CS_VREDRAW; // стиль окна
    wcex.lpfnWndProc = WndProc; // указатель на оконную процедуру
    wcex.cbClsExtra = 0; // количество дополнительных байтов, которые размещаются после структуры класса окна
    wcex.cbWndExtra = 0; // количество дополнительных байтов, которые размещаются после структуры окна
    wcex.hInstance = hInstance; // дескриптор экземпляра приложения
    wcex.hIcon = LoadIcon(hInstance, IDI_APPLICATION); // дескриптор значка
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW); // дескриптор курсора
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1); // дескриптор кисти фона
    wcex.lpszMenuName = nullptr; // имя меню
    wcex.lpszClassName = L"FinanceTracker"; // имя класса окна 
    wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION); // дескриптор маленького значка

    // Регистрация класса окна
    if (!RegisterClassExW(&wcex)) {
        MessageBoxW(NULL, L"Не удалось зарегистрировать класс окна.", L"Ошибка", MB_OK | MB_ICONERROR); // показываем сообщение об ошибке
        return 1;
    }

    // Создание окна приложения
    HWND hWnd = CreateWindowW(L"FinanceTracker", L"Отслеживание финансов", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 400, 250, nullptr, nullptr, hInstance, nullptr); // создаем окно приложения

    if (!hWnd) { // если окно не создано
        MessageBoxW(NULL, L"Не удалось создать окно приложения.", L"Ошибка", MB_OK | MB_ICONERROR); // показываем сообщение об ошибке
        return 1; // возвращаем код ошибки
    }

    // Отображение окна
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd); // обновляем окно

    // Цикл сообщений
    MSG msg; // структура сообщения
    while (GetMessageW(&msg, nullptr, 0, 0)) { // получаем сообщение
        TranslateMessage(&msg); // транслируем сообщение
        DispatchMessageW(&msg); // отправляем сообщение
    }

    return (int)msg.wParam; // возвращаем код выхода
}


