int romanToInt(char* s) {
    int sum = 0;
    int last = -1;
    for (int i = 0; i < strlen(s); i++) {
        switch (s[i]) {
        case 'I':
            sum+=1;
            break;
        case 'V':
            if(last == 'I'){
                sum += 3;
            } else {
                sum+=5;
            }
            break;
        case 'X':
            if(last == 'I'){
                sum += 8;
            } else {
                sum+=10;
            }
            break;
        case 'L':
            if(last == 'X'){
                sum += 30;
            } else {
                sum+=50;
            }
            break;
        case 'C':
            if(last == 'X'){
                sum += 80;
            } else {
                sum+=100;
            }
            break;
        case 'D':
            if(last == 'C'){
                sum += 300;
            } else {
                sum+=500;
            }
            break;
        case 'M':
            if(last == 'C'){
                sum += 800;
            } else {
                sum+=1000;
            }
            break;
        }
        last = s[i];
    }
    return sum;
}
