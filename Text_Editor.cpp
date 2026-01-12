#include <iostream>
#include <fstream>
#include <conio.h>
#include <filesystem>
#include <Windows.h>
#include <vector>

void saveFile(const std::vector<std::string>& buffer, const std::string& path) {
    std::ofstream outFile(path);
    if (outFile.is_open()) {
        for (const auto& line : buffer) {
            outFile << line << "\n"; // Добавляем перенос строки!
        }
        outFile.close();
        std::cout << "\nFile saved successfully!";
        Sleep(1000); // Чтобы пользователь успел увидеть надпись
    }
}

void render(std::vector<std::string> text_buffer, int cursor_line) {
    system("cls");
    for (int i = 0; i < text_buffer.size(); ++i) {
        if (i == cursor_line) {
            std::cout << "> " << text_buffer[i] << " <" << std::endl;
        }
        else {
            std::cout << "  " << text_buffer[i] << std::endl;
        }
    }
    std::cout << "\n--- [Arrows to navigate | Esc to exit] ---";
}

int editor(std::vector<std::string> text_buffer, std::string filePath){
    int cursorLine = 0;
    std::string result;

    if (text_buffer.empty()) text_buffer.push_back("");

    while (true) {
        render(text_buffer, cursorLine);

        int key = _getch();

        if (key == 0 || key == 224) {
            key = _getch();
            if (key == 72 && cursorLine > 0) cursorLine--;
            else if (key == 80 && cursorLine < text_buffer.size() - 1) cursorLine++; 
        }
        else if (key == 27) { // Esc
            while (true) {
                system("cls");
                std::cout << "Exit? (Ctrl+Q: Quit without saving | Ctrl+S: Save and Quit)" << std::endl;

                int exitKey = _getch(); 

                if (exitKey == 19) { // Ctrl + S
                    saveFile(text_buffer, filePath);
                    return 0;
                }
                else if (exitKey == 17) { // Ctrl + Q
                    return 0;
                }
                else if (exitKey == 27) { 
                    break;
                }
            }
        }
        else if (key == 8) { // Backspace
            if (!text_buffer[cursorLine].empty()) {
                text_buffer[cursorLine].pop_back();
            }
        }
        else if (key == 13) { // Enter
            text_buffer.insert(text_buffer.begin() + cursorLine + 1, "");
            cursorLine++;
        }
        else if (key == 19) { // Ctrl + S
            std::ofstream outFile(filePath);
            for (const std::string& word : text_buffer) result += word;
            outFile << result;
        }
        else if (key >= 32 && key <= 126) {
            text_buffer[cursorLine] += (char)key;
        }
    }
    return 0;
}


int main()
{
    std::string filePath;
    std::string fileName;

    std::vector<std::string> text_buffer;


    while (true) {
        system("cls");
        std::cout << "--------------------------------------------\n" << std::endl;
        std::cout << "Open File: Ctrl + O | New File: Ctrl + N\n";
        std::cout << "--------------------------------------------\n" << std::endl;
        if (_kbhit()) {
            char key = _getch();

            if (key == 15) {
                std::cout << "Type the file path that you want to open." << "\n" << "Example: C:/Program Files/test.txt" << std::endl;
                std::cin >> filePath;

                if (std::filesystem::exists(filePath)) {
                    std::ifstream file(filePath);
                    std::string line;
                    text_buffer.clear();

                    while (std::getline(file, line)) {
                        text_buffer.push_back(line);
                    }
                    editor(text_buffer, filePath);
                }
                else std::cout << "File not found!\n";

            } else if (key == 14) {
                std::cout << "File will be created in this directory type a name." << "\n" << "Example: my_file.txt" << std::endl;
                std::cin >> fileName;

                std::ofstream file(fileName);
            }
        }
        Sleep(10);
    }
}
