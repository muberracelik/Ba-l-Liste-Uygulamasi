#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct sehir
{
    int plakaKodu;
    int komsuSayisi;
    char *sehirAdi;
    char *bolge;
    struct komsu *komsuluk;
    struct sehir *next;
    struct sehir *prev;
};

struct komsu
{
    char *komsuAdlari;
    char *komsubolgeleri;
    int kplakakodu;
    struct komsu *next;
};

struct sehir* start=NULL;
struct sehir* temp=NULL;
struct sehir* tempPlaka=NULL;
struct komsu* tempKomsu=NULL;
int satir_sayisi;
FILE *cikti;


void dosyadanOkuma()
{
    FILE *sehirler=fopen("sehirler.txt","r");

    char line[100];
    char *token;
    int i;
    int komsuSayisi;
    satir_sayisi=0;

    while(fgets(line, 100, sehirler)!=NULL)
    {
        struct sehir *dosyadakiSehirler=(struct sehir*)malloc(sizeof(struct sehir));//her sehir icin bir dugum olusturdum, asagida bu dugumu baglayip, verileri dugume ekledim.
        if(start==NULL)
        {
            start=dosyadakiSehirler;
            start->next=NULL;
        }
        else
        {
            temp=start;
            while(temp->next!=NULL)
            {

                temp=temp->next;
            }

            temp->next=dosyadakiSehirler;
            dosyadakiSehirler->prev=temp;
            dosyadakiSehirler->next=NULL;

        }

        strtok(line,"\n");
        satir_sayisi++;

        token=strtok(line, ",");
        dosyadakiSehirler->plakaKodu = atoi(token);

        int satir_uzunlugu = 1;

        while( token != NULL )
        {

            // printf( " %s", token );
            token = strtok(NULL, ",");

            if (token == NULL)
            {
                break;

            }
            satir_uzunlugu++;
            int uzunluk=strlen(token);

            if(satir_uzunlugu==2)
            {
                char *sehirAdlari=(char *)malloc(strlen(token)+1);
                strcpy(sehirAdlari,token);
                dosyadakiSehirler->sehirAdi=sehirAdlari;

            }
            if(satir_uzunlugu==3)
            {
                char *bolgeAdlari=(char *)malloc(strlen(token)+1);
                strcpy(bolgeAdlari,token);
                dosyadakiSehirler->bolge=bolgeAdlari;
            }


            if(satir_uzunlugu>3)
            {
                struct komsu *sehirlerinKomsulari=(struct komsu*)malloc(sizeof(struct komsu));//her komsu icin bir dugum olusturdum, asagida bu dugumu baglayip, verileri dugume ekledim

                char *komsuAdi=(char *)malloc(strlen(token)+1);
                strcpy(komsuAdi,token);
                sehirlerinKomsulari->komsuAdlari=komsuAdi;
                if(tempKomsu==NULL)
                {
                    dosyadakiSehirler->komsuluk=sehirlerinKomsulari;
                    sehirlerinKomsulari->next=NULL;
                    tempKomsu=sehirlerinKomsulari;

                }
                else
                {
                    tempKomsu=dosyadakiSehirler->komsuluk;
                    while(tempKomsu->next!=NULL)
                    {

                        tempKomsu=tempKomsu->next;
                    }
                    tempKomsu->next=sehirlerinKomsulari;
                    sehirlerinKomsulari->next=NULL;


                }

            }

            dosyadakiSehirler->komsuSayisi = satir_uzunlugu-3;

        }

        tempKomsu=NULL;

    }

      fclose(sehirler);
      // printf("%d",satir_sayisi);
}

int karsi(char *bir, char *iki)
{
    char *tmp1=(char *)malloc(strlen(bir)+1);
    strcpy(tmp1,bir);

    char *tmp2=(char *)malloc(strlen(iki)+1);
    strcpy(tmp2,iki);
    return strcmp(tmp1,tmp2);


}

void komsuPlakaAtama()
{
    temp=start; // işlem yaptığımız sehri tutar.
    for(int i=0; i<satir_sayisi; i++)
    {
        tempPlaka=start; // tempPlaka her komsuluk plakasi icin sehir listesini gezer.
        for(int j=0; j<satir_sayisi; j++)
        {
            if(karsi(tempPlaka->sehirAdi,temp->komsuluk->komsuAdlari)==0)
            {
                temp->komsuluk->kplakakodu=tempPlaka->plakaKodu;
                temp->komsuluk->komsubolgeleri=tempPlaka->bolge;
                break;
            }
            tempPlaka=tempPlaka->next;
        }
        tempKomsu=temp->komsuluk->next;
        while(tempKomsu!=NULL)
        {
            tempPlaka=start;
            for(int j=0; j<satir_sayisi; j++)
            {
                if(karsi(tempPlaka->sehirAdi,tempKomsu->komsuAdlari)==0)
                {
                    tempKomsu->kplakakodu=tempPlaka->plakaKodu;
                    tempKomsu->komsubolgeleri=tempPlaka->bolge;

                    break;
                }
                tempPlaka=tempPlaka->next;
            }

            tempKomsu=tempKomsu->next;
        }

        temp=temp->next;
    }

}

void sehirEkleme(char *sehir, int plaka, char *bolge)
{
    struct sehir *yeniSehir=(struct sehir*)malloc(sizeof(struct sehir));
    if(start!=NULL)
    {
        temp=start;
        while(temp->next!=NULL)
        {
            if(temp->plakaKodu==plaka)   // araya sehir ekleme
            {
                printf("Eklemek istediginiz sehir mecvuttur. Lutfen farkli bir sehir ekleyiniz.\n");
                free(yeniSehir);
                return;
            }
            temp=temp->next;
        }
    }


    char *sehirAdi=(char *)malloc(strlen(sehir)+1);
    strcpy(sehirAdi,sehir);
    yeniSehir->sehirAdi=sehirAdi;

    char *bolgeAdi=(char *)malloc(strlen(bolge)+1);
    strcpy(bolgeAdi,bolge);
    yeniSehir->bolge=bolge;


    yeniSehir->plakaKodu=plaka;


    if(start==NULL)   // listede hic sehir yoksa ekleme
    {
        start=yeniSehir;
        start->next=NULL;
    }
    else
    {

        if(start->plakaKodu > yeniSehir->plakaKodu)   // basına sehir ekleme
        {
            start->prev=yeniSehir;
            yeniSehir->next=start;
            start=yeniSehir;
            return;
        }

        temp=start;
        while(temp->next!=NULL)
        {
            if(temp->plakaKodu > yeniSehir->plakaKodu)   // araya sehir ekleme
            {
                yeniSehir->prev=temp->prev;
                yeniSehir->next=temp;
                temp->prev->next=yeniSehir;
                temp->prev=yeniSehir;
                return;
            }
            temp=temp->next;
        }
        // sonuna sehir ekleme
        temp->next=yeniSehir;
        yeniSehir->prev=temp;
        yeniSehir->next=NULL;
    }
}
void plakaKoduAra(int plakaKodu)
{
    int bulunduMu=0;
    temp=start;
    while(temp!=NULL)
    {
        if(temp->plakaKodu==plakaKodu)
        {
            printf("%d %s %s\n",temp->plakaKodu,temp->sehirAdi,temp->bolge);
            printf("  | ^\n  | |\n  | |\n  | |\n  v |\n");


            fprintf(cikti,"%d %s %s\n",temp->plakaKodu,temp->sehirAdi,temp->bolge);
            fprintf(cikti,"  | ^\n  | |\n  | |\n  | |\n  v |\n");

            bulunduMu=1;
            if(temp->komsuluk!=NULL)
            {
                printf("\t\t ------->%d %s %s\n",temp->komsuluk->kplakakodu,temp->komsuluk->komsuAdlari,temp->komsuluk->komsubolgeleri);
                fprintf(cikti,"\t\t ------->%d %s %s\n",temp->komsuluk->kplakakodu,temp->komsuluk->komsuAdlari,temp->komsuluk->komsubolgeleri);
            }
            else
            {
                return;
            }
            break;
        }
        temp=temp->next;
    }
    if(bulunduMu==1)
    {
        tempKomsu=temp->komsuluk->next;
        while(tempKomsu!=NULL)
        {
            printf("\t\t ------->%d %s %s\n",tempKomsu->kplakakodu,tempKomsu->komsuAdlari,tempKomsu->komsubolgeleri);
            fprintf(cikti,"\t\t ------->%d %s %s\n",tempKomsu->kplakakodu,tempKomsu->komsuAdlari,tempKomsu->komsubolgeleri);
            tempKomsu=tempKomsu->next;
        }
    }
    else
    {
        printf("Bu plaka kodunda sehir bulunmamaktadir.Eklemek ister misiniz?\n");
        printf("Evet icin E , Hayir icin H harfine basiniz.");
        char cevap;
        scanf("%s",&cevap);
        cevap=toupper(cevap);
        if(cevap=='H')
        {
            return;
        }
        if(cevap=='E')
        {
            char sehirIsmi[20], bolgeIsmi[3]; // bolgeIsminin 3 olmasinin sebebi 2 karakter kullanimindan sonra bitiş içinde bir karakter almamız gerekir.
            int plakaKodu;

            printf("Eklemek istediginiz sehirlerin bilgilerini giriniz.\n");
            printf("Sehir ismi:");
            scanf("%s",&sehirIsmi);
            printf("Sehrin Plaka Kodu:");
            scanf("%d",&plakaKodu);
            printf("Sehrin Bolgesi:");
            scanf("%s",&bolgeIsmi);
            sehirEkleme(sehirIsmi,plakaKodu,bolgeIsmi);
        }
    }

}



void sehirAra(char *sehir)
{
    temp=start;
    int bulunduMu=0;
    while(temp!=NULL)
    {
        if(karsi(temp->sehirAdi,sehir)==0)
        {
            printf("%d %s %s\n",temp->plakaKodu,temp->sehirAdi,temp->bolge);
            fprintf(cikti,"%d %s %s\n",temp->plakaKodu,temp->sehirAdi,temp->bolge);
            bulunduMu=1;
            if(temp->komsuluk!=NULL)
            {
                printf("\t\t ------->%d %s %s\n",temp->komsuluk->kplakakodu,temp->komsuluk->komsuAdlari,temp->komsuluk->komsubolgeleri);
                fprintf(cikti,"\t\t ------->%d %s %s\n",temp->komsuluk->kplakakodu,temp->komsuluk->komsuAdlari,temp->komsuluk->komsubolgeleri);
            }
            else
            {
                return;
            }
            break;

        }

        temp=temp->next;
    }
    if(bulunduMu==1)
    {
        tempKomsu=temp->komsuluk->next;
        while(tempKomsu!=NULL)
        {
            printf("\t\t ------->%d %s %s\n",tempKomsu->kplakakodu,tempKomsu->komsuAdlari,tempKomsu->komsubolgeleri);
            fprintf(cikti,"\t\t ------->%d %s %s\n",tempKomsu->kplakakodu,tempKomsu->komsuAdlari,tempKomsu->komsubolgeleri);
            tempKomsu=tempKomsu->next;
        }
    }
    else
    {
        printf("Bu isimde sehir bulunmamaktadir.Eklemek ister misiniz?\n");
        printf("Evet icin E , Hayir icin H harfine basiniz.");
        char cevap;
        scanf("%s",&cevap);
        cevap=toupper(cevap);
        if(cevap=='H')
        {
            return;
        }
        if(cevap=='E')
        {
            char sehirIsmi[20], bolgeIsmi[3]; // bolgeIsminin 3 olmasinin sebebi 2 karakter kullanimindan sonra bitiş içinde bir karakter almamız gerekir.
            int plakaKodu;

            printf("Eklemek istediginiz sehirlerin bilgilerini giriniz.\n");

            printf("Sehrin Plaka Kodu:");
            scanf("%d",&plakaKodu);

            printf("Sehir ismi:");
            scanf("%s",&sehirIsmi);

            printf("Sehrin Bolgesi:");
            scanf("%s",&bolgeIsmi);

            sehirEkleme(sehirIsmi,plakaKodu,bolgeIsmi);
            yazdir();
        }
    }
}

void bolgeAra(char *bolge)
{
    temp=start;
    while(temp!=NULL)
    {
        if(karsi(temp->bolge,bolge)==0)
        {
            printf("%d %s %d\n",temp->plakaKodu,temp->sehirAdi,temp->komsuSayisi);
            fprintf(cikti,"%d %s %d\n",temp->plakaKodu,temp->sehirAdi,temp->komsuSayisi);
        }

        temp=temp->next;
    }

}

void komsuSayiAra(int sayi)
{
    temp=start;
    while(temp!=NULL)
    {
        if(temp->komsuSayisi>=sayi)
        {
            printf("%d %s %s %d\n",temp->plakaKodu,temp->sehirAdi,temp->bolge,temp->komsuSayisi);
            fprintf(cikti,"%d %s %s %d\n",temp->plakaKodu,temp->sehirAdi,temp->bolge,temp->komsuSayisi);
        }

        temp=temp->next;
    }

}
void sehirSil(char *sehir)
{
    temp=start;
    int silindi=0;
    while(temp!=NULL)   // silinecek sehir ilk dugumse
    {
        if(karsi(temp->sehirAdi,sehir)==0)
        {
            if(temp==start)
            {
                temp->next->prev=NULL;
                start=temp->next;
                silindi=1;
                break;

            }
            else if(temp->next==NULL)     // silinecek sehir son dügümse
            {
                temp->prev->next=NULL;
                silindi=1;
                break;

            }
            else     // silinecek sehir ara dugumse
            {
                temp->prev->next=temp->next;
                temp->next->prev=temp->prev;
                silindi=1;
                break;

            }

        }
        temp=temp->next;

    }
    if (silindi==1)
    {
        printf("\n%s isimli sehir silindi\n",temp->sehirAdi);
        komsuSil(temp->plakaKodu);
        free(temp);
    }
    else
    {
        printf("Silmek istediginiz sehir bulunamadi.\n");
    }



}


void komsuSil(int plaka)
{
    struct komsu* tempSil=NULL;
    temp=start;
    while(temp!=NULL)
    {
        if(temp->komsuluk->kplakakodu==plaka)
        {
            temp->komsuluk=temp->komsuluk->next;
            temp->komsuSayisi--;
        }
        tempKomsu=temp->komsuluk->next;
        tempSil=temp->komsuluk;
        while(tempKomsu!=NULL)
        {

            if(tempKomsu->kplakakodu==plaka)
            {
                if(tempKomsu->next==NULL)
                {
                    tempSil->next=NULL;
                    free(tempKomsu);
                    temp->komsuSayisi--;
                    break;
                }
                tempSil->next=tempKomsu->next;
                free(tempKomsu);
                temp->komsuSayisi--;
                break;

            }
            tempKomsu=tempKomsu->next;
            tempSil=tempSil->next;

        }
        temp=temp->next;
    }

}
yazdir()
{
    temp=start;
    while(temp!=NULL)
    {

        plakaKoduAra(temp->plakaKodu);

        temp=temp->next;
    }


}
int main()
{
    cikti=fopen("cikti.txt","a");
    int secim;
    int secim1;
    char sehirIsimSecim[20];
    char bolgeSecim[3];
    int plakaKoduSecim;
    int komsuSayisi;
    int komsuSil2;
    char sehirSil2[20];

        dosyadanOkuma();
        komsuPlakaAtama();
    while(1)
    {

        printf("Dosyadaki sehirleri listelemek icin 1:\n");
        printf("Herhangi bir sehiri aramak icin 2:\n");
        printf("Herhangi bir bolgede bulunan sehirleri listelemek icin 3:\n");
        printf("Belli bir komsu sayisi kriterine uyan sehirleri listelemek icin 4:\n");
        printf("Yeni bir sehir veya komsuluk eklemek icin 5:\n");
        printf("Sehir veya komsuluk kayitlarini silmek icin 6:\n");
        printf("Programdan cikmak icin 0:\ntusuna basiniz.");
        printf("\n");
        scanf("%d",&secim);
        switch(secim)
        {
        case 0:
            exit(0);

        case 1:
            fputs("SECIM 1:\n",cikti);
            yazdir();
            break;


        case 2:
           fputs("SECIM 2:\n",cikti);
            printf("Sehir ismine gore aramak icin 1:\n");
            printf("Plaka koduna gore aramak icin 2: tuslayiniz.");
            scanf("%d",&secim1);
            if(secim1==1)
            {
                printf("Aramak istediginiz sehrin ismi:");
                scanf("%s",&sehirIsimSecim);
                sehirAra(sehirIsimSecim);


            }
            else
            {
                printf("Aramak istediginiz sehrin plaka kodu:");
                scanf("%d",&plakaKoduSecim);
                plakaKoduAra(plakaKoduSecim);

            }
            break;

        case 3:
            fputs("SECIM 3:\n",cikti);
            printf("Aramak istediginiz bolge:");
            scanf("%s",&bolgeSecim);
            bolgeAra(bolgeSecim);

            break;

        case 4:
            fputs("SECIM 4:\n",cikti);
            printf("Komsu sayisina gore listelemek istediginiz sehirler icin bir sayi giriniz.");
            scanf("%d",&komsuSayisi);
            komsuSayiAra(komsuSayisi);
            break;

        case 5:
            fputs("SECIM 5:\n",cikti);
            printf("Eklemek istediginiz sehrin bilgilerini giriniz.\n");
            printf("Sehrin ismi:");
            scanf("%s",&sehirIsimSecim);
            printf("Sehrin plaka kodu:");
            scanf("%d",&plakaKoduSecim);
            printf("Sehrin bolgesi:");
            scanf("%s",&bolgeSecim);
            sehirEkleme(sehirIsimSecim,plakaKoduSecim,bolgeSecim);
            break;

        case 6:
            fputs("SECIM 6:\n",cikti);
            printf("Sehir silmek icin 1:\n");
            printf("Komsu silmek icin 2: tuslayiniz.");
            scanf("%d",&secim1);

            if(secim1==1)
            {
                printf("Silmek istediginiz sehrin ismi:");
                scanf("%s",&sehirSil2);
                sehirSil(sehirSil2);



            }
            else
            {
                printf("Silmek istediginiz komsulugun plakasi:");
                scanf("%d",&komsuSil2);
                komsuSil(komsuSil2);


            }
            break;

        }
    }



    fclose(cikti);
}
