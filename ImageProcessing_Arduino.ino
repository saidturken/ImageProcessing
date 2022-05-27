char incomingByte; // BANDDAKİ VERİLERİ TUTTUĞUMUZ DEĞİŞKENİMİZ

// KONTROL DEĞİŞKENLERİ
int x = 0;
int k1 = 0;
int k2 = 0;
int kx = 0;

// AÇISAL TARAMA YAPACAK MOTOR TANIMLAMASI.

#define Y_STEP_PIN         60 
#define Y_DIR_PIN          61
#define Y_ENABLE_PIN       56

// NETLİK MOTORU TANIMLAMASI.

#define Q_STEP_PIN         36
#define Q_DIR_PIN          34
#define Q_ENABLE_PIN       30

void setup() {
  Serial.begin(9600); // HABERLEŞME BANDI 9600 OLARAK BELİRLENDİ.

  // MOTORLAR ÇIKIŞ ÜRETECEĞİNDEN OUTPUT OLARAK ATANDI.
  pinMode(Y_STEP_PIN  , OUTPUT);
  pinMode(Y_DIR_PIN    , OUTPUT);
  pinMode(Y_ENABLE_PIN    , OUTPUT);

  pinMode(Q_STEP_PIN, OUTPUT);
  pinMode(Q_DIR_PIN, OUTPUT);
  pinMode(Q_ENABLE_PIN, OUTPUT);

  digitalWrite(Y_ENABLE_PIN, HIGH); //HIGH OLURSA MOTOR ÇALIŞMAZ. BUNU YAPMA SEBEBİMİZ İŞLEM OLMADIĞI ZAMAN MOTORA BOŞUNA GÜÇ GİDİP ISINMAMASI.
  digitalWrite(Q_ENABLE_PIN, HIGH); //HIGH OLURSA MOTOR ÇALIŞMAZ. BUNU YAPMA SEBEBİMİZ İŞLEM OLMADIĞI ZAMAN MOTORA BOŞUNA GÜÇ GİDİP ISINMAMASI.
}

void loop() {
  if (Serial.available() > 0) // BAND ÜZERİNDEN VERİ GELİRSE İF BLOĞUNA GİRİYORUZ.
  {
    incomingByte = Serial.read(); // PYTHON KODUNDAN GELEN VERİLERİ OKUYORUZ.
    if (incomingByte == 'T' && k1 == 0) // GELEN VERİ 'T' İSE YAKLAŞIK 30 DERECELİK BİR KONUMA GİDİYOR.
    {
      digitalWrite(Y_ENABLE_PIN, LOW); // MOTORA GÜÇ GİTMESİ İÇİN 'LOW' DEĞERİNİ VERDİK.
      digitalWrite(Y_DIR_PIN, HIGH); // MOTORUN DÖNÜŞ YÖNÜ BELİRLENDİ. HIGH: SOL - LOW: SAĞ
      for (int i = 0; i < 40; i++) // STEP MOTORU 40 ADIM OLARAK DÖNDÜRÜYORUZ
      {
        digitalWrite(Y_STEP_PIN, HIGH);
        delayMicroseconds(5000); // MOTORUN DÖNÜŞ HIZI BU DEĞER ARTTIKÇA AZALIR.
        digitalWrite(Y_STEP_PIN, LOW);
        delayMicroseconds(5000);
      }
      digitalWrite(Y_ENABLE_PIN, HIGH); // MOTORLA İŞLEM BİTTİKTEN SONRA MOTORA GİDEN GÜCÜ KESİYORUZ.
      k1 = 1;
      delay(2000);
    }
    if (incomingByte == 'P') // GELEN VERİ 'P' İSE BİR SONRAKİ BÖLGEYE HAREKETİNİ GERÇEKLEŞTİRİYOR.
    {
      if (x < 10) { // ALACAĞIMIZ GÖRÜNTÜ SAYISINI BELİRLİYORUZ
        digitalWrite(Y_ENABLE_PIN, LOW);
        digitalWrite(Y_DIR_PIN, LOW);
        for (int i = 0; i < 10; i++) // !!! DİKKAT. BURADAKİ ADIM DEĞERİ ALACAĞIMIZ GÖRÜNTÜ ADEDİNE GÖRE DEĞİŞTİRİLMELİ. GÖRÜNTÜ ADEDİ AZALDIKÇA ARTMASI GEREKLİDİR.
        {
          digitalWrite(Y_STEP_PIN, HIGH);
          delayMicroseconds(5000);
          digitalWrite(Y_STEP_PIN, LOW);
          delayMicroseconds(5000);
        }
        digitalWrite(Y_ENABLE_PIN, HIGH);
        delay(2000);
        Serial.print('1'); // BAZI İŞLEMLERİN SONUNDA PYTHON A BAND ÜZERİNDEN VERİ GÖNDERİLMEKTE. BU GİDEN VERİYE GÖRE PYTHON KODUNDA İŞLEM YAPILMAKTA.
        x = x + 1; // HER GÖRÜNTÜ ALDIĞIMIZDA ARTTIRIYORUZ
      }
      else //EĞER BELİRLEDİĞİMİZ GÖRÜNTÜ SAYISINI ALDI İSE '0' DEĞERİNİ BAND ÜZERİNDEN PYTHON KODUNA GÖNDERİYORUZ.
      {
        Serial.print('0');
      }
    }
    if (incomingByte == 'S' && k2 == 0) // GELEN VERİ 'S' İSE 0 DERECEYE DÖNDÜRÜYORUZ.
    {
      digitalWrite(Y_ENABLE_PIN, LOW);
      digitalWrite(Y_DIR_PIN, HIGH);
      for (int i = 0; i < 40; i++)
      {
        digitalWrite(Y_STEP_PIN, HIGH);
        delayMicroseconds(5000);
        digitalWrite(Y_STEP_PIN, LOW);
        delayMicroseconds(5000);
      }
      digitalWrite(Y_ENABLE_PIN, HIGH);
      Serial.print('2');
      k2 == 1;

    }
    if (incomingByte == 'X' && kx == 0) // GELEN VERİ 'X' İSE NETLİK MOTORUNU 5 BİRİM UZAKLAŞTIRIYORUZ.
    {
      kx = 1;
      digitalWrite(Q_ENABLE_PIN, LOW);
      digitalWrite(Q_DIR_PIN, HIGH);
      for (int j = 0; j < 5; j++) { // 5 KERE
        for (int i = 0; i < 5; i++) // 5 ADIM MOTOR HAREKETİ SAĞLIYORUZ
        {
          digitalWrite(Q_STEP_PIN, HIGH);
          delayMicroseconds(5000);
          digitalWrite(Q_STEP_PIN, LOW);
          delayMicroseconds(5000);
        }
        delay(100);
      }
      digitalWrite(Q_ENABLE_PIN, HIGH);
      delay(1500);
      Serial.print('1'); // BAZI İŞLEMLERİN SONUNDA PYTHON A BAND ÜZERİNDEN VERİ GÖNDERİLMEKTE. BU GİDEN VERİYE GÖRE PYTHON KODUNDA İŞLEM YAPILMAKTA.
    }
    if (incomingByte == 'U') // GELEN VERİ 'U' İSE NETLİK MOTORUNU 10 BİRİM UZAKLAŞTIRIYORUZ.
    {
      digitalWrite(Q_ENABLE_PIN, LOW);
      digitalWrite(Q_DIR_PIN, HIGH);
      for (int j = 0; j < 10; j++) { // 10 KERE
        for (int i = 0; i < 5; i++) // 5 ADIM MOTOR HAREKETİ SAĞLIYORUZ
        {
          digitalWrite(Q_STEP_PIN, HIGH);
          delayMicroseconds(5000);
          digitalWrite(Q_STEP_PIN, LOW);
          delayMicroseconds(5000);
        }
        delay(500);
      }
      digitalWrite(Q_ENABLE_PIN, HIGH);
      delay(1000);
    }
    if (incomingByte == 'Y') // GELEN VERİ 'Y' İSE NETLİK MOTORUNU 1 BİRİM YAKINLAŞTIRIYORUZ.
    {
      digitalWrite(Q_ENABLE_PIN, LOW);
      digitalWrite(Q_DIR_PIN, LOW);
      for (int i = 0; i < 5; i++) // 5 ADIM MOTOR HAREKETİ SAĞLIYORUZ
      {
        digitalWrite(Q_STEP_PIN, HIGH);
        delayMicroseconds(5000);
        digitalWrite(Q_STEP_PIN, LOW);
        delayMicroseconds(5000);
      }
      digitalWrite(Q_ENABLE_PIN, HIGH);
      delay(1000);
      Serial.print('1');
      
    }
    if (incomingByte == 'L') // GELEN VERİ 'Y' İSE NETLİK MOTORUNU 5 BİRİM YAKINLAŞTIRIYORUZ.
    {
      digitalWrite(Q_ENABLE_PIN, LOW);
      digitalWrite(Q_DIR_PIN, LOW);
      for (int j = 0; j < 5; j++) { // 5 KERE
        for (int i = 0; i < 5; i++) // 5 ADIM MOTOR HAREKETİ SAĞLIYORUZ
        {
          digitalWrite(Q_STEP_PIN, HIGH);
          delayMicroseconds(5000);
          digitalWrite(Q_STEP_PIN, LOW);
          delayMicroseconds(5000);
        }
        delay(500);
      }
      digitalWrite(Q_ENABLE_PIN, HIGH);
      delay(1000);
    }
  }
}
