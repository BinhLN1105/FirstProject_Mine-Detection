#include <SFML/Graphics.hpp> //Dung thư viện sfml(Thư viện đồ họa) cho trò chơi
#include <time.h>
#include <sstream>
using namespace sf;

int main()
{
    srand(time(0));

    RenderWindow app(VideoMode(400, 400), "Tro choi do min");

    int w = 32;
    /*Mảng grid được sử dụng để lưu trữ thông tin về vị trí của các mìn và các ô không chứa mìn. */
    int grid[10][10];
    /*Mảng sgrid được sử dụng để hiển thị các ô trên cửa sổ đồ họa của trò chơi.*/
    int sgrid[10][10]; 
    int mines=10;    // Sô mìn có trong trò chơi 
    int remainingMines = mines;
    int placedMines = 0;
    Texture t;
    t.loadFromFile("images.jpg"); //Load ảnh chứa mìn và các số
    Sprite s(t);

    //tạo một lưới cho trò chơi với kích thước 10x10, 
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            sgrid[i][j] = 10;
            grid[i][j] = 0; // Ban đầu, tất cả các ô không có mìn
        }
    }
    // Đặt mìn ngẫu nhiên trong bảng
    while (placedMines < 10)
    {
        int randX = rand() % 10;
        int randY = rand() % 10;

        if (grid[randX][randY] != 9) // Đảm bảo ô chưa có mìn
        {
            grid[randX][randY] = 9;
            placedMines++;
        }
    }
    /*Đoạn này tính toán số lượng mìn xung quanh mỗi ô không phải là mìn .*/
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            int n = 0;
            if (grid[i][j] == 9)
                continue;
            for (int dx = -1; dx <= 1; dx++)
            {
                for (int dy = -1; dy <= 1; dy++)
                {
                    if (i + dx >= 0 && i + dx < 10 && j + dy >= 0 && j + dy < 10 && grid[i + dx][j + dy] == 9)
                        n++;
                }
            }
            grid[i][j] = n;
        }
    }
//Khởi tạo 2 biến điều kiện win và lose.
    bool youWin = false;
    bool youLose = false;

    while (app.isOpen())
    {
        //Điều chỉnh màn hình chơi.
        Vector2u windowSize = app.getSize();
        int gridWidth = 10 * w;
        int gridHeight = 10 * w;

        int xOffset = (windowSize.x - gridWidth) / 2;
        int yOffset = (windowSize.y - gridHeight) / 2;

        Vector2i pos = Mouse::getPosition(app);
        int x = (pos.x - xOffset) / w;
        int y = (pos.y - yOffset) / w;

        Event e;
        while (app.pollEvent(e))
        {
            if (e.type == Event::Closed)
                app.close();

            if (!youWin && !youLose)
            {
                if (e.type == Event::MouseButtonPressed && e.mouseButton.button == Mouse::Left)
                {
                    if (sgrid[x][y] == 10)
                    {
                        if (grid[x][y] == 9)
                        {
                            youLose = true;
                            remainingMines = 0; // Không còn mìn sau khi thua
                            // Hiển thị các quả mìn trong trò chơi.
                            for (int i = 0; i < 10; i++)
                            {
                                for (int j = 0; j < 10; j++)
                                {
                                    if (grid[i][j] == 9)
                                        sgrid[i][j] = grid[i][j];
                                }
                            }
                        }
                        else
                        {
                            // Hiển thị ô và các ô lân cận
                            sgrid[x][y] = grid[x][y];
                            if (grid[x][y] == 0)
                            {
                                // Thuật toán Flood-fill để hiển thị các ô kề nhau
                                for (int i = -1; i <= 1; i++)
                                {
                                    for (int j = -1; j <= 1; j++)
                                    {
                                        if (grid[x + i][y + j] == 0 && sgrid[x + i][y + j] == 10)
                                        {
                                            sgrid[x + i][y + j] = grid[x + i][y + j];
                                        }
                                    }
                                }
                            }
                        }
                    }

                    // Kiểm tra điều kiện win
                    youWin = true;
                    for (int i = 0; i < 10; i++)
                    {
                        for (int j = 0; j < 10; j++)
                        {
                            if (sgrid[i][j] == 10 && grid[i][j] != 9)
                            {
                                youWin = false;
                                break;
                            }
                        }
                        if (!youWin)
                            break;
                    }
                }
                else if (e.type == Event::MouseButtonPressed && e.mouseButton.button == Mouse::Right)
                {
                    if (sgrid[x][y] == 10 && remainingMines > 0)
                    {
                        sgrid[x][y] = 11;
                        remainingMines--;
                    }
                    else if (sgrid[x][y] == 11)
                    {
                        sgrid[x][y] = 10;
                        remainingMines++;
                    }
                }
            }
            else if (youWin || youLose)
            {
                if (e.type == Event::MouseButtonPressed && e.mouseButton.button == Mouse::Left)
                {
                    // Click để replay
                    youWin = false;
                    youLose = false;
                    placedMines=0;
                    remainingMines = mines;

                    // reset lại bảng mới
                    for (int i = 0; i < 10; i++)
                        {
                            for (int j = 0; j < 10; j++)
                            {
                                sgrid[i][j] = 10;
                                grid[i][j] = 0;
                            }
                        }

                        while (placedMines < 10)
                        {
                            int randX = rand() % 10;
                            int randY = rand() % 10;

                            if (grid[randX][randY] != 9)
                            {
                                grid[randX][randY] = 9;
                                placedMines++;
                            }
                        }

                    // Tính toán lại số lượng xung quanh mìn
                    for (int i = 0; i < 10; i++)
                        {
                            for (int j = 0; j < 10; j++)
                            {
                                int n = 0;
                                if (grid[i][j] == 9)
                                    continue;
                                for (int dx = -1; dx <= 1; dx++)
                                {
                                    for (int dy = -1; dy <= 1; dy++)
                                    {
                                        if (i + dx >= 0 && i + dx < 10 && j + dy >= 0 && j + dy < 10 && grid[i + dx][j + dy] == 9)
                                            n++;
                                    }
                                }
                                grid[i][j] = n;
                            }
                        }
                    }
                }
            }

        app.clear(Color::White);//Nền ngoài

        for (int i = 0; i < 10; i++)
        {
            for (int j = 0; j < 10; j++)
            {
                s.setTextureRect(IntRect(sgrid[i][j] * w, 0, w, w));
                s.setPosition(xOffset + i * w, yOffset + j * w);
                app.draw(s);
            }
        }

        Font font;
        font.loadFromFile("US101.TTF"); // Dùng font chữ cho trò chơi

        // Hiển thị số lượng mìn có trong trò chơi
        Text remainingMinesText;
        remainingMinesText.setFont(font);
        remainingMinesText.setCharacterSize(20);
        remainingMinesText.setFillColor(Color::Black);
        std::stringstream ss;
        ss << "So min co trong tro choi: " << remainingMines;
        remainingMinesText.setString(ss.str());
        remainingMinesText.setPosition(10, 10);

        // Hiển thị dòng thông báo cảm ơn
        Text thankYouText;
        thankYouText.setFont(font);
        thankYouText.setCharacterSize(20);
        thankYouText.setFillColor(Color::Blue);
        thankYouText.setString("Thank you for playing!");
        thankYouText.setPosition(app.getSize().x / 2 - thankYouText.getGlobalBounds().width / 2,
                                app.getSize().y - thankYouText.getGlobalBounds().height - 10);

        app.draw(remainingMinesText);
        app.draw(thankYouText);

        if (youLose)// Khi thua
        {
            Text youLoseText;
            youLoseText.setFont(font);
            youLoseText.setCharacterSize(50);
            youLoseText.setFillColor(Color::Red);
            youLoseText.setString("You Lose!");
            youLoseText.setPosition(app.getSize().x / 2 - youLoseText.getGlobalBounds().width / 2,
                                    app.getSize().y / 2 - youLoseText.getGlobalBounds().height / 2);

            Text playAgainText;
            playAgainText.setFont(font);
            playAgainText.setCharacterSize(25);
            playAgainText.setFillColor(Color::Black);
            playAgainText.setString("Click to play again.");
            playAgainText.setPosition(app.getSize().x / 2 - playAgainText.getGlobalBounds().width / 2,
                                    youLoseText.getPosition().y + youLoseText.getGlobalBounds().height + 10);

            app.draw(youLoseText);
            app.draw(playAgainText);
        }
        else if (youWin)// Khi thắng
        {
            Text youWinText;
            youWinText.setFont(font);
            youWinText.setCharacterSize(50);
            youWinText.setFillColor(Color::Green);
            youWinText.setString("You Win!");
            youWinText.setPosition(app.getSize().x / 2 - youWinText.getGlobalBounds().width / 2,
                                   app.getSize().y / 2 - youWinText.getGlobalBounds().height / 2);
            
            Text playAgainText;
            playAgainText.setFont(font);
            playAgainText.setCharacterSize(25);
            playAgainText.setFillColor(Color::Black);
            playAgainText.setString("Click to play again.");
            playAgainText.setPosition(app.getSize().x / 2 - playAgainText.getGlobalBounds().width / 2,
                                    youWinText.getPosition().y + youWinText.getGlobalBounds().height + 10);
                                    
            app.draw(youWinText);
            app.draw(playAgainText);
        }

        app.display();
    }

    return 0;
}
