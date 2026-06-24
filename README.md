# Snake Oyunu

Klasik Snake oyununun C++ ile yazılmış modern bir uyarlaması.

## 📋 Proje Hakkında

Bu proje, SFML (Simple and Fast Multimedia Library) kullanarak C++ ile geliştirilmiş, klasik Snake oyunudur. Yılanı kontrol ederek meyveleri yiyip, puanınızı arttırabilir ve oyun alanında kalarak en yüksek skora ulaşabilirsiniz.

## 🎮 Oyun Özellikleri

- **Klasik Mekanikler**: Yılanı kontrol et, meyve ye, büyü
- **Puan Sistemi**: Aldığın her meyve için puan kazan
- **En Yüksek Puan Takibi**: Oyun oturumunda en iyi skorunu göster
- **Duraklatma Özelliği**: Oyunu istediğin zaman duraklat
- **Farklı Hız Seviyeleri**: Oyun zorluğu dinamik olarak ayarlanır

## 📁 Proje Yapısı

```
snake_cpp/
├── main.cpp           # Ana oyun kodu
└── images/            # Oyun grafikleri
    ├── green.png      # Yılan grafikleri
    ├── red.png        # Meyve grafikleri
    └── white.png      # Arka plan/diğer öğeler
```

## 🛠️ Gereksinimler

- C++11 veya daha yeni sürüm
- SFML 2.5+ kütüphanesi

## 🚀 Kurulum ve Çalıştırma

### macOS'ta

```bash
# SFML kütüphanesini yükle (Homebrew ile)
brew install sfml

# Projeyi derle (g++ ile, önerilen yöntem)
g++ main.cpp -o snake_game -std=c++17 -I/opt/homebrew/include -L/opt/homebrew/lib -lsfml-graphics -lsfml-window -lsfml-system

# Oyunu çalıştır
./snake_game
```

**Alternatif: clang++ ile derleme**

```bash
clang++ -c main.cpp -o main.o
clang++ main.o -o snake -lsfml-graphics -lsfml-window -lsfml-system
./snake
```

### Doğrudan Çalıştırma

Eğer proje zaten derlenmiş ise, basitçe çalıştırılabilir dosyayı çalıştır:

```bash
./snake
```

## 🎮 Oyun Kontrolleri

| Tuş | Fonksiyon |
|-----|-----------|
| **↑ ↓ ← →** | Yılanı harekete geçir |
| **P** | Oyunu duraklat/devam et |
| **Esc** | Oyundan çık |

## 📊 Oyun Mekanikleri

- **Harita Boyutu**: 30x20 grid
- **Başlangıçta**: Yılanın 4 parçası
- **Meyve**: Her yenildiğinde yılan 1 parça büyür
- **Oyun Bitişi**: Yılan duvar veya kendisiyle çarparsa

## 💡 İpuçları

- Yılanı kontrol ederken keskin manevraları çabuk yap
- Meyveleri toplama sırasında duvarlardan uzak kal
- Yüksek skorlar için uzun bir yılan oluştur

---

**Dil**: C++  
**Kütüphane**: SFML 2.5+
