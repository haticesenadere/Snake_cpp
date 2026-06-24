#include <SFML/Graphics.hpp>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <optional>
#include <string>

using namespace sf;

//oyun haritası boyutları
int N = 30;
int M = 20;

int size = 16;

int w = size * N;
int h = size * M;

//yılan ayarları
// 0 = Aşağı
// 1 = Sol
// 2 = Sağ
// 3 = Yukarı

int dir = 2;
int nextDir = 2;
int num = 4;
const int MAX_SNAKE_SIZE = 100;

int score = 0;
int highScore = 0;

bool gameOver = false;
bool paused = false;
bool won = false;
float delay = 0.10f;

struct Snake
{
    int x;
    int y;
} s[MAX_SNAKE_SIZE];

struct Fruit
{
    int x;
    int y;
} f;

//pozisyon yılanın üzerinden  mi ? 
bool IsOnSnake(int x, int y)
{
    for (int i = 0; i < num; i++)
    {
        if (s[i].x == x && s[i].y == y)
            return true;
    }

    return false;
}
// yeni meyve oluşturma 

void SpawnFruit()
{
    // Meyve, yılanın üzerinde oluşmayana kadar
    // yeni koordinat üretiyoruz.
    do
    {
        f.x = rand() % N;
        f.y = rand() % M;

    } while (IsOnSnake(f.x, f.y));
}

void ResetGame()
{
    dir = 2;
    nextDir = 2;

    num = 4;

    score = 0;

    delay = 0.10f;

    gameOver = false;
    paused = false;
    won = false;

    // Yılanın başlangıç konumu
    for (int i = 0; i < num; i++)
    {
        s[i].x = 10;
        s[i].y = 10 + i;
    }

    SpawnFruit();
}

//başlık güncelleme 
void UpdateWindowTitle(RenderWindow& window)
{
    std::string title;

    title = "Snake Game | Skor: ";
    title += std::to_string(score);
    title += " | En Yuksek: ";
    title += std::to_string(highScore);

    if (paused)
    {
        title += " | DURAKLATILDI - Devam etmek icin SPACE";
    }

    if (gameOver && !won)
    {
        title += " | OYUN BITTI - Yeniden baslatmak icin R";
    }

    if (won)
    {
        title += " | KAZANDIN! - Yeniden baslatmak icin R";
    }

    window.setTitle(title);
}

//yulanı hareket ettirme  

void Tick()
{
    if (gameOver || paused)
        return;

    // Büyüme sırasında kullanmak için kuyruğun
    // son konumunu saklıyor 
    Snake oldTail = s[num - 1];

    // Yılanın gövdesini öndeki parçayı takip
    // edecek şekilde hareket ettioyurz .
    for (int i = num - 1; i > 0; i--)
    {
        s[i].x = s[i - 1].x;
        s[i].y = s[i - 1].y;
    }

    // Bekleyen yönü mevcut yön yaptık 
    dir = nextDir;

    // Yılanın başını hareket ettik 
    if (dir == 0)
        s[0].y += 1;

    if (dir == 1)
        s[0].x -= 1;

    if (dir == 2)
        s[0].x += 1;

    if (dir == 3)
        s[0].y -= 1;

    // ekranın diğer tarafına geçiş 

    if (s[0].x >= N)
        s[0].x = 0;

    if (s[0].x < 0)
        s[0].x = N - 1;

    if (s[0].y >= M)
        s[0].y = 0;

    if (s[0].y < 0)
        s[0].y = M - 1;

    // yılanın kendine çarpma durumu 

    for (int i = 1; i < num; i++)
    {
        if (s[0].x == s[i].x &&
            s[0].y == s[i].y)
        {
            gameOver = true;

            if (score > highScore)
                highScore = score;

            return;
        }
    }

    // meyve yedi m i ? 

    if (s[0].x == f.x &&
        s[0].y == f.y)
    {
        score += 10;

        if (score > highScore)
            highScore = score;

        // Yılanı büyüt.
        if (num < MAX_SNAKE_SIZE)
        {
            s[num] = oldTail;
            num++;
        }

        // Her meyvede oyun biraz hızlanır.
        // Ancak aşırı hızlı olmaması için
        // minimum 0.04 saniyede sınırlandırıyoruz.
        delay = std::max(0.04f, delay - 0.004f);

        // Maksimum uzunluğa ulaşıldıysa oyuncu kazanır.
        if (num >= MAX_SNAKE_SIZE)
        {
            won = true;
            gameOver = true;
            return;
        }

        SpawnFruit();
    }
}

int main()
{
    srand(static_cast<unsigned int>(time(nullptr)));

    RenderWindow window(
        VideoMode(Vector2u(w, h)),
        "Snake Game"
    );

    // İşlemciyi gereksiz yere zorlamaması için
    // görüntü yenileme hızını sınırlıyoruz.
    window.setFramerateLimit(60);

    // Bir tuşa basılı tutulduğunda sürekli
    // KeyPressed olayı oluşmasını engeller.
    window.setKeyRepeatEnabled(false);

    Texture t1;
    Texture t2;

    if (!t1.loadFromFile("images/white.png"))
    {
        std::cerr
            << "Hata: images/white.png yuklenemedi."
            << std::endl;

        return 1;
    }

    if (!t2.loadFromFile("images/red.png"))
    {
        std::cerr
            << "Hata: images/red.png yuklenemedi."
            << std::endl;

        return 1;
    }

    Sprite sprite1(t1);
    Sprite sprite2(t2);

    // Görseller 16x16 değilse bile her hücreye
    // sığacak şekilde boyutlandırıyoruz.

    Vector2u texture1Size = t1.getSize();
    Vector2u texture2Size = t2.getSize();

    sprite1.setScale(
        Vector2f(
            static_cast<float>(size) /
                static_cast<float>(texture1Size.x),

            static_cast<float>(size) /
                static_cast<float>(texture1Size.y)
        )
    );

    sprite2.setScale(
        Vector2f(
            static_cast<float>(size) /
                static_cast<float>(texture2Size.x),

            static_cast<float>(size) /
                static_cast<float>(texture2Size.y)
        )
    );

    // Oyun duraklatıldığında veya bittiğinde
    // ekranın üzerine çizilecek yarı saydam katman.
    RectangleShape darkOverlay(
        Vector2f(
            static_cast<float>(w),
            static_cast<float>(h)
        )
    );

    darkOverlay.setFillColor(
        Color(0, 0, 0, 150)
    );

    Clock clock;

    float timer = 0.0f;

    ResetGame();

    //ana oyun döngüüs 

    while (window.isOpen())
    {
        float elapsedTime =
            clock.restart().asSeconds();

        timer += elapsedTime;

        //event kontrolü 

        while (const std::optional event = window.pollEvent())
        {
            // Pencere kapatma tuşuna basıldı.
            if (event->is<Event::Closed>())
            {
                window.close();
            }

            // Pencerenin odağı kaybolduğunda
            // oyun otomatik olarak duraklatılır.
            if (event->is<Event::FocusLost>())
            {
                if (!gameOver)
                    paused = true;
            }

            if (const auto* keyPressed =
                    event->getIf<Event::KeyPressed>())
            {
                Keyboard::Key key = keyPressed->code;

                // ESC ile oyundan çık.
                if (key == Keyboard::Key::Escape)
                {
                    window.close();
                }

                // R ile oyunu yeniden başlat.
                if (key == Keyboard::Key::R)
                {
                    ResetGame();
                    timer = 0.0f;
                }

                // SPACE ile duraklat/devam ettir.
                if (key == Keyboard::Key::Space &&
                    !gameOver)
                {
                    paused = !paused;
                    timer = 0.0f;
                }

                // Oyun durmuşsa veya bitmişse
                // hareket komutlarını alma.
                if (paused || gameOver)
                    continue;

                // yön kontrolleri 

                // SOL
                // Yılan sağa gidiyorsa doğrudan sola dönemez.
                if ((key == Keyboard::Key::Left ||
                     key == Keyboard::Key::A) &&
                    dir != 2)
                {
                    nextDir = 1;
                }

                // SAĞ
                // Yılan sola gidiyorsa doğrudan sağa dönemez.
                if ((key == Keyboard::Key::Right ||
                     key == Keyboard::Key::D) &&
                    dir != 1)
                {
                    nextDir = 2;
                }

                // YUKARI
                // Yılan aşağı gidiyorsa doğrudan yukarı dönemez.
                if ((key == Keyboard::Key::Up ||
                     key == Keyboard::Key::W) &&
                    dir != 0)
                {
                    nextDir = 3;
                }

                // AŞAĞI
                // Yılan yukarı gidiyorsa doğrudan aşağı dönemez.
                if ((key == Keyboard::Key::Down ||
                     key == Keyboard::Key::S) &&
                    dir != 3)
                {
                    nextDir = 0;
                }
            }
        }

        // oyun güncellemesi 

        if (!paused &&
            !gameOver &&
            timer >= delay)
        {
            // Bilgisayar kısa süreliğine yavaşlarsa
            // zamanın tamamen kaybolmaması için
            // timer = 0 yapmak yerine delay çıkarıyoruz.
            timer -= delay;

            Tick();
        }

        UpdateWindowTitle(window);

        // çizim 

        window.clear(Color::Black);

        // Oyun alanındaki kareleri çiz.
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < M; j++)
            {
                sprite1.setPosition(
                    Vector2f(
                        static_cast<float>(i * size),
                        static_cast<float>(j * size)
                    )
                );

                window.draw(sprite1);
            }
        }

        // yılanı çiz 

        for (int i = 0; i < num; i++)
        {
            sprite2.setPosition(
                Vector2f(
                    static_cast<float>(s[i].x * size),
                    static_cast<float>(s[i].y * size)
                )
            );

            window.draw(sprite2);
        }

        // meyveyi çizz 

        sprite2.setPosition(
            Vector2f(
                static_cast<float>(f.x * size),
                static_cast<float>(f.y * size)
            )
        );

        window.draw(sprite2);

        // Oyun durmuşsa veya bitmişse
        // ekranı karart.
        if (paused || gameOver)
        {
            window.draw(darkOverlay);
        }

        window.display();
    }

    return 0;
}