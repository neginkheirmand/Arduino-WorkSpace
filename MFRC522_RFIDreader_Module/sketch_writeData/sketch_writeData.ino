String pass = "";
String key = "";
String vigenereCoded="";
void setup() {
  Serial.begin(9600);
}

char vigenere(char a, char key){
    char arr[]={'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z' };
    int posA=-1;
    int posKey=-1;
    for(int i=0; i<(sizeof(arr)); i++){
        if(a==arr[i]){
            posA=i;
        }
        if(key==arr[i]){
            posKey=i;
        }
        if(posKey!=-1 && posA!=-1){
            break;
        }
    }
    return arr[(posA+posKey)%(sizeof(arr))];
}



void loop() {
  Serial.println("start now? Press 1");
    while(Serial.available() == 0){}
  String input = Serial.readString();
  if(input=="1\n"){
    Serial.println("Please enter the key?");
    while(Serial.available() == 0){}    
    key = Serial.readString();
    Serial.println("Please enter the pass?");
    while(Serial.available() == 0){}  
    pass = Serial.readString();
    //in arduino .length() function returns the size of the string +1
    //length() in cpp   =      length() in arduino - 1
    //why? cause the function readString() also reads the \n in the end of the input
    for(int i=0; i<pass.length()-1; i++){
        vigenereCoded += vigenere(pass[i], key[i%(key.length()-1)]);
    }
    Serial.println("The Code:"+vigenereCoded);
    
  }
  
}
