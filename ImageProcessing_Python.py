import serial # Arduino ile haberleşmek için kullandığımız kütüphane.
import cv2 # Görüntü toplamak için Opencv kütüphanesini kullanıyoruz.

portname = input("Port Adını Giriniz: ") # Arduino port adını giriyoruz. Her arduino bağlandığında değişebilen bir ad.
cap = cv2.VideoCapture(0) # Bağlı olan kameradan görüntü almak için kullandığımız fonksiyon. 0 olursa dahili, 1 olursa harici kameradan görüntüleri alır.
global counter,cimage
counter = 1 # Kaydedilen görüntülerin numarası.
cimage = 0 # Her bölgede alınacak birden fazla fotoğrafın numarası
fmdizi = [0,0,0,0,0,0,0,0,0,0,0] # Her bölgede alınan fotoğrafların netlik değerini tutmak için oluşturduğumuz dizi. Varsayılan değer başta 0 olarak atandı.
port = serial.Serial(portname,9600,timeout=1) # 'portname' adlı porta bağlan. 9600 bandında haberşelme gerçekleştir. 1 saniyelik aralıklarla.
print(port.isOpen()) # Arduino haberleşmesi için oluşturduğumuz portun haberleşmesini başlatmak için kullanıyoruz.

port.flushInput() # Haberleşme bandındaki verileri sıfırlar.
while True:
    port.write("T".encode()) # Arduino ya 'T' verisini gönderiyoruz. Bu 'T' verisine göre Arduino işlemler gerçekleştiriyor.(İlk 30 Dereceye Gitme)
    port.write("X".encode()) # Arduino ya 'X' verisini gönderiyoruz. Bu 'X' verisine göre Arduino işlemler gerçekleştiriyor.(Netlik Motoru Uzaklaştırma 5 Birim)
    veri = port.read() # Arduino bazı işlemleri bitirdikten sonra bize geri veri göndermekte bu veriyi okumak için bu fonksiyonu kullanıyoruz.
    print(veri)
    if(veri != b''): # Porttaki boş veri b'' olarak tanımlı. Bundan farklı bir veri gelirse if bloğuna giriş sağlıyoruz.
        while True:
            if(veri == b'1'): # Arduinodan gelen veri 0 a eşit ise:
                if(cimage < 10): # Her bölgede 10 görüntü alıyoruz
                    print("Veri 1 Geldi")
                    success, image = cap.read() # Kameradan görüntüyü alıyoruz.
                    gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY) # Görünütüyü siyah-beyaz olarak dönüştürüyoruz.
                    fm = cv2.Laplacian(gray, cv2.CV_64F).var() # Projedeki netlik konusundaki asıl nokta burası. Laplacian kütüphanesi alınan görünütüleri işleyerek matematiksel bir değer geri döndürüyor.
                    fmdizi[cimage] = fm # Bu değerleri başta oluşturduğumuz diziye aktarıyoruz.
                    print(fm)
                    imgname = "c" + str(cimage) + ".jpg" # Görüntü hangi ad ile kayıt edileceğini belirliyoruz.
                    cv2.imwrite("C:/Users/Ebu Belk AKYILDIZ/Desktop/CaptureFull/" + imgname, image) # Bir bölgedeki aldığımız görüntüleri daha sonra kullanmak için kayıt ediyoruz.
                    cimage += 1 # Bölgedeki görüntü sayacını arttırıyoruz.
                    port.write("Y".encode()) # Arduino ya 'Y' verisini gönderiyoruz. Bu 'Y' verisine göre Arduino işlemler gerçekleştiriyor.(Netlik Motoru Yakınlaştırma 1 Birim)
                else:
                    cimage = 0 # Bölgedeki görüntüleri aldıktan sonra bu değeri sıfırlıyoruz. Tekrar kullanmak için
                    imgname = "c" + str(fmdizi.index(max(fmdizi))) + ".jpg" # Burada netlik değerleri tutulan dizideki maksimum değer olan görüntünün adını oluşturuyoruz.
                    img = cv2.imread("C:/Users/Ebu Belk AKYILDIZ/Desktop/CaptureFull/" + imgname) # Bölgedeki alınan fotoğraflar arasından en net olan fotoğrafı kaydettiğimiz noktadan çekiyoruz.
                    imgname = "capture" + str(counter) + ".jpg" # Görüntü hangi ad ile kayıt edileceğini belirliyoruz.
                    cv2.imwrite("C:/Users/Ebu Belk AKYILDIZ/Desktop/Capture/" + imgname, img) # Tek bir bölgedeki en net fotoğrafı kayıt ediyoruz.
                    counter += 1 # Sonraki bölgenin fotoğraf adı için sayacı arttırıyoruz.
                    port.write("U".encode()) # Arduino ya 'U' verisini gönderiyoruz. Bu 'U' verisine göre Arduino işlemler gerçekleştiriyor.(Netlik Motoru Uzaklaştırma 10 Birim)
                    port.write("P".encode()) # Arduino ya 'P' verisini gönderiyoruz. Bu 'P' verisine göre Arduino işlemler gerçekleştiriyor.(Bir Sonraki Bölgeye Geçiş)
            if(veri == b'0'): # Arduinodan gelen veri 0 a eşit ise:
                print("Veri 0 Geldi")
                port.write("S".encode()) # Arduino ya 'S' verisini gönderiyoruz. Bu 'S' verisine göre Arduino işlemler gerçekleştiriyor.(0 Dereceye Dönme)
            if (veri == b'2'):
                print("Veri 2 Geldi")
                port.write("L".encode()) # Arduino ya 'L' verisini gönderiyoruz. Bu 'L' verisine göre Arduino işlemler gerçekleştiriyor.(Netlik Motoru Uzaklaştırma 5 Birim)(Netlik Motoru Başladığı Konuma Döner)
                break # Program sonlandı döngülerden çıkıyoruz.
            print(veri)
            veri = port.read()
        break # Program sonlandı döngülerden çıkıyoruz.
print("Görüntüler Alındı")
port.close() # Arduino haberleşmesi bittiğinde port bağlantısını sonlandırıyoruz.