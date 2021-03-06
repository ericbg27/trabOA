#include <stdio.h>
#include "drive_lib.h"

track_array cylinder[10];
fatlist_s fatlist_sectors[3000];
fatent_s *fatlist_files_initial=NULL;

void show_menu () { // simples função que limpa a tela e mostra o menu.

  system("clear");
  printf("1 - Escrever Arquivo\n2 - Ler Arquivo\n3 - Apagar Arquivo\n4 - Mostrar Tabela FAT\n5 - Sair\n");
  return;
}

void menu_selection (int menu_option) { // função que direciona o programa para as funções dependendo da opção escolhida.

  switch (menu_option){
    case 1:
      write_file();
      break;
    case 2:
      break;
    case 3:
      break;
    case 4:
      break;
    case 5:
      break;
    default:
      break;
  }
  return;
}

int write_file(){
  FILE *file;
  char file_name[100], c;
  int t=0, s=0, i=0, j=0, fat_sector, fat_sector_search;
  fatent_s *fatlist_files_actual, *fatlist_files_new;

  do{
    printf("Informe o nome do arquivo, com '.txt'.\n");
    fgets(file_name, sizeof(file_name), stdin);
    file=fopen(file_name, "r");
    system("clear");
  }while(file!=NULL);
  printf("Arquivo aberto com sucesso.\n");
  while(cylinder[j].track[t].sector[s*4].bytes_s[0]!=0){
    s++;
    if(s==15){
      j++;
      s=0;
      if(j==10){
        t++;
        j=0;
        if(t==5){
          printf("Disco cheio\n");
          return 0;
        }
      }
    }
  }
  fat_sector=find_fat_sector(j, t, s);
  fatlist_files_actual=(fatent_s*)malloc(sizeof(fatlist_files_actual));
  if (fatlist_files_initial==NULL){
    fatlist_files_initial=(fatent_s*)malloc(sizeof(fatlist_files_initial));
    fatlist_files_actual=fatlist_files_initial;
  }
  else{/*se a lista fat já está inicializada*/
    fatlist_files_actual=fatlist_files_initial;
    do{/*move atual até o fim da lista*/
      fatlist_files_actual=fatlist_files_actual->next_file;
    }while(fatlist_files_actual->next_file!=NULL);  
  }
  fatlist_files_new=(fatent_s*)malloc(sizeof(fatlist_files_new));
  strcpy(fatlist_files_new->file_name, file_name);
  fatlist_files_new->first_sector=fat_sector;
  fatlist_files_actual->next_file=fatlist_files_new;
  fatlist_files_new->first_sector=fat_sector;
  fat_sector_search=fat_sector;
  while((c=fgetc(file)!=EOF)){
    cylinder[j].track[t].sector[s].bytes_s[i]=c;
    if(fatlist_sectors[fat_sector].used==0){fatlist_sectors[fat_sector].used=1;}
    i++;
    if(i==512){
      s++;
      fat_sector_search++;
      while(fatlist_sectors[fat_sector_search].used==1){
        s++;
        fat_sector_search++;
      }

      i=0;
      if(s==60){
        j++;
        s=0;
        if(j==10){
          t++;
          j=0;
          if(t==5){
            printf("Disco cheio, dados truncados.\n");
            return 0;
          }
        }
      }
    }
  }
  return 1;
}

int find_fat_sector(int j, int t, int s){
  int sum;
  sum=(j*300)+(t*60)+s;
  return sum;
}

/*int main (){
  int menu_option=0;

  show_menu();
  scanf("%d", &menu_option);
  while ((menu_option!=1)
      &&(menu_option!=2)
      &&(menu_option!=3)
      &&(menu_option!=4)
      &&(menu_option!=5)) { // garantia de entrada correta.
    show_menu();
    printf("Escolha invalida!\nEntre novamente.\n");
    scanf("%d", &menu_option);
  }
  menu_selection(menu_option);
  return 0;
}*/