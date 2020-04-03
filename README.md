# SoalShiftSISOP20_modul3_B07

## Soal 3

### Fungsi untuk mencari ekstensi dari suatu file :
```
const char *cari_ekstensi(const char *nama_file){
	char *ekstensi = strrchr (nama_file, '.');
	if (ekstensi == null)
	return "unknown";
	return ekstensi + 1;
}
```

### Fungsi untuk memindahkan file ke lokasi lain :
```
void *bikin_folder(void *arg){

fgets(source, MAX, stdin);
source[strlen(source)- 1] = '\0';

fgets(dest, MAX, stdin);
dest[strlen(target) - 1] = '\0';

file1 = fopen(source, "r");
file2 = fopen(dest, "w");

while((ch = fgetc(file1)) != EOF){
	fputc(ch, file2);
}

fclose(file1);
fclose(file2);

return 0;
}
```

### Fungsi untuk menampilkan file di suatu directory :
```
void *list_folder (void *arg){

d = opendir(".");

if (d){
while ((dir = readdir(d)) != NULL){
	printf("%s\n", dir->d_name);
}
closedir(d);
}
return(0);
}
```
