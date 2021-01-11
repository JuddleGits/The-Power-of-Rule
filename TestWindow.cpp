#include <windows.h>
#include <string>
#include <ctime>
#include <cctype>
#include <sstream>
#include <iostream>

using namespace std;

void AddControls(HWND hwnd);
void ManageControls(HWND hwnd, WPARAM wParam);
string problem();
string solve(string input);

const char g_szClassName[] = "myWindowClass";

HWND button1, button2, text1, logo, instructions, question, textBox1, textBox2;
HBITMAP image1, image2;
char answer[40];
string insert, result, finish = "Number Right: ";
int gwtstat1 = 0, numRight = 0;
clock_t start;
clock_t stop;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		case WM_CREATE:
			image1 = (HBITMAP)LoadImage(NULL, TEXT("Power_Logo.bmp"),
				IMAGE_BITMAP, 0, 0,
				LR_LOADFROMFILE);
			image2 = (HBITMAP)LoadImage(NULL, TEXT("Instructions.bmp"),
				IMAGE_BITMAP, 0, 0,
				LR_LOADFROMFILE);
            AddControls(hwnd);
            break;
        case WM_COMMAND:
        	ManageControls(hwnd, wParam);
        	break;
		case WM_CLOSE:
			DestroyWindow(hwnd);
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hwnd, msg, wParam, lParam);
	}

	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wc;
	HWND hwnd;
	MSG Msg;
	
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = 0;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = g_szClassName;
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	if (!(RegisterClassEx(&wc)))
	{
		MessageBox (NULL, "Window Registration Failed", "Error!",
			MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	hwnd = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		g_szClassName,
		"The Power of Rule",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 805, 675,
		NULL, NULL, hInstance, NULL);

	if (hwnd == NULL)
	{
		MessageBox (NULL, "Window Creation Failed!", "Error!",
			MB_ICONEXCLAMATION | MB_OK);
	}

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	while (GetMessage(&Msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}

	return Msg.wParam;
}

void AddControls(HWND hwnd)
{
	logo = CreateWindow(TEXT("static"), NULL,
		WS_VISIBLE | WS_CHILD | SS_BITMAP,
		90, 100, 600, 300,
		hwnd, NULL, NULL, NULL);
	SendMessage(logo, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)image1);

	button1 = CreateWindow(TEXT("button"), TEXT("Start Game"),
			WS_VISIBLE | WS_CHILD,
			235, 450, 80, 30,
			hwnd, (HMENU)1, NULL, NULL);

	button2 = CreateWindow(TEXT("button"), TEXT("Instructions"),
			WS_VISIBLE | WS_CHILD,
			435, 450, 85, 30,
			hwnd, (HMENU)2, NULL, NULL);
}

void ManageControls(HWND hwnd, WPARAM wParam)
{
	switch(LOWORD(wParam))
	{
		case 1:
			DestroyWindow(logo);
			DestroyWindow(button1);
			DestroyWindow(button2);

			insert = problem();

			question = CreateWindow(TEXT("static"), insert.c_str(),
				WS_VISIBLE | WS_CHILD | SS_CENTER,
				240, 300, 200, 30,
				hwnd, NULL, NULL, NULL);

			textBox1 = CreateWindow(TEXT("edit"), NULL,
				WS_VISIBLE | WS_CHILD | WS_BORDER,
				240, 330, 200, 30,
				hwnd, NULL, NULL, NULL);

			button1 = CreateWindow(TEXT("button"), TEXT("Enter"),
				WS_VISIBLE | WS_CHILD,
				460, 330, 80, 30,
				hwnd, (HMENU)4, NULL, NULL);

			start = clock();
			break;
		case 2:
			DestroyWindow(logo);
			DestroyWindow(button1);
			DestroyWindow(button2);
			
			instructions = CreateWindow(TEXT("static"), NULL,
				WS_VISIBLE | WS_CHILD | SS_BITMAP,
				0, 100, 795, 336,
				hwnd, NULL, NULL, NULL);
			SendMessage(instructions, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)image2);

			button1 = CreateWindow(TEXT("button"), TEXT("<-- Back"),
				WS_VISIBLE | WS_CHILD,
				355, 450, 80, 30,
				hwnd, (HMENU)3, NULL, NULL);
			break;
		case 3:
			DestroyWindow(button1);
			DestroyWindow(instructions);
			AddControls(hwnd);
			break;
		case 4:
			gwtstat1 = GetWindowText(textBox1, answer, 40);

			if (gwtstat1 != 0)
			{
				string answerStr = answer;
				string correctAnswer = solve(insert);
				if (correctAnswer.compare(answerStr) != 0)
				{
					if (textBox2 != NULL)
						DestroyWindow(textBox2);

					textBox2 = CreateWindow(TEXT("static"), TEXT("Wrong Answer, Try Again"),
						WS_VISIBLE | WS_CHILD | SS_CENTER,
						240, 250, 200, 30,
						hwnd, NULL, NULL, NULL);
				}

				else
				{
					numRight++;
					if (textBox2 != NULL)
						DestroyWindow(textBox2);

					textBox2 = CreateWindow(TEXT("static"), TEXT("Correct Answer!"),
						WS_VISIBLE | WS_CHILD | SS_CENTER,
						240, 250, 200, 30,
						hwnd, NULL, NULL, NULL);
					
					DestroyWindow(textBox1);
					textBox1 = CreateWindow(TEXT("edit"), NULL,
						WS_VISIBLE | WS_CHILD | WS_BORDER,
						240, 330, 200, 30,
						hwnd, NULL, NULL, NULL);

					insert = problem();
					DestroyWindow(question);
					question = CreateWindow(TEXT("static"), insert.c_str(),
						WS_VISIBLE | WS_CHILD | SS_CENTER,
						240, 300, 200, 30,
						hwnd, NULL, NULL, NULL);
				}
			}

			else
			{
				::MessageBox(hwnd, "Please Insert An Answer", "Empty", MB_OK | MB_ICONEXCLAMATION);
			}

			stop = clock() - start;
			if ((stop / (double)CLOCKS_PER_SEC) >= 60.0)
			{
				DestroyWindow(question);
				DestroyWindow(textBox1);
				DestroyWindow(textBox2);
				DestroyWindow(button1);
				result = to_string(numRight);
				finish.append(result);
				numRight = 0;

				textBox1 = CreateWindow(TEXT("static"), finish.c_str(),
					WS_VISIBLE | WS_CHILD,
					315, 250, 120, 20,
					hwnd, NULL, NULL, NULL);

				button1 = CreateWindow(TEXT("button"), TEXT("Main Menu"),
					WS_VISIBLE | WS_CHILD,
					260, 350, 80, 30,
					hwnd, (HMENU)5, NULL, NULL);

				button2 = CreateWindow(TEXT("button"), TEXT("Play Again"),
					WS_VISIBLE | WS_CHILD,
					400, 350, 90, 30,
					hwnd, (HMENU)6, NULL, NULL);

				finish = "Number Right: ";
			}
			break;
		case 5:
			DestroyWindow(question);
			DestroyWindow(textBox1);
			DestroyWindow(textBox2);
			DestroyWindow(button1);
			DestroyWindow(button2);
			AddControls(hwnd);
			break;
		case 6:
			DestroyWindow(textBox1);
			DestroyWindow(textBox2);
			DestroyWindow(button1);
			DestroyWindow(button2);
			ManageControls(hwnd, (WPARAM)1);
			break;
	}
}

string problem()
{
	int number, term, sign;
	string termStorage, tempStorage;

	srand(time(NULL));

    number = (rand() % 3) + 2;
    for (int i = 0; i < number; i++)
    {
    	term = (rand() % 10) + 1;

    	if (i < (number - 1))
    	{
    		if (term == 1)
    			tempStorage = "x^" + to_string(number - i);

    		else
    			tempStorage = to_string(term) + "x^" + to_string(number - i);
    	}

    	else
    	{
    		if (term == 1)
    			tempStorage = "x";
    		
    		else
    			tempStorage = to_string(term) + "x";
    	}
        
        if (i < (number - 1))
        {
        	sign = rand() % 2;

        	if (sign == 0)
        		termStorage += tempStorage + " + ";

       	    else
        		termStorage += tempStorage + " - ";
        }

        else
        	termStorage += tempStorage; 

        tempStorage.clear();
    }

    return termStorage;
}

string solve(string input)
{
	string storage, answer, temp;
	stringstream get;
    int number, mult = 1, newNum;

    int i = 0;
    
    while (true)
    {
        while ((input[i] != 'x') && (i < (int)input.length()))
        {
            if (isdigit(input[i]))
                storage += input[i];
            i++;
        }

        if (storage.empty())
            storage = "1";

        if ((input[i] != 'x') && (answer.empty())) 
            return "0";

        else if (storage == "0")
            return "0";

        else
        {
            get << storage;
            get >> number;
            get.clear();
            i++;

            if ((input[i] == '^') && (i < (int)input.length()))
            {
                storage.clear();
                i++;

                while ((input[i] != ' ') && (i < (int)input.length()))
                {
                    storage += input[i];
                    i++;
                }

                get << storage;
                get >> mult;
                get.clear();
            }

            newNum = number * mult;
            mult--;

            if (mult > 1)
                answer += to_string(newNum) + "x^" + to_string(mult);

            else if (mult == 1)
                answer += to_string(newNum) + "x";

            else
                answer += to_string(newNum);

            if (i >= (int)input.length())
                return answer;

            else
            {
                i++;
                mult = 1;

                if ((input[i] == '+') || (input[i] == '-'))
                {
                    temp = input[i];
                    answer += " " + temp + " ";
                    temp.clear();
                    storage.clear();
                    i += 2;
                }
            }
        }
    }
}