#include <stdio.h>
#include <stdbool.h>
#include <time.h>

/* prototype */
int genQuestions(int difficulty);
bool answerQuestion(int value);
void response(bool isCorrect);
int rand();

int main(void)
{
 int questionNum;
 int diffLevel;
 int i;
 int countScore;
 int calcValue;
 bool isCorrectAnswer;

while(true)
{
	printf("How many questions for this test (1-20)?");
 	scanf("%d", &questionNum);
  	if(questionNum > 0 && questionNum <= 20)
  	{
   		break;
  	}
}

while(true)
{
 	printf("Select difficulty (1-4):");
  	scanf("%d", &diffLevel);
  	if(diffLevel > 0 && diffLevel <= 4)
  	{
  	 	break;
 	}
}

countScore = 0;
calcValue = 0;
isCorrectAnswer = false;

for(i = 1; i <= questionNum; i ++)
 {
   /* print question */
   	printf("Question %d:", i);
 	calcValue = genQuestions(diffLevel);
   /* get answer */
   	isCorrectAnswer = answerQuestion(calcValue);
   /* print response */
   	response(isCorrectAnswer);
   	if(isCorrectAnswer == false)
   	{
    		printf("The correct answer was: %d\n", calcValue);
   	} else {
		countScore++;
	}
 }

 printf("Your score was %d/%d\n", countScore, i);
 return 1;
}

int genQuestions(int difficulty)
{
	int operand1, operand2;
 	int opValue; /* = rand() % 4 + 1;*/
 	int actualValue;
 	char* operat;
	opValue = 1;
 
 /* generate random numbers based on selected difficulty level */ 
 switch (difficulty){
  case 1: 
         operand1 = rand() % 10 + 1;
         operand2 = rand() % 10 + 1;
         break;
  case 2:
         operand1 = rand() % 50 + 1;
         operand2 = rand() % 50 + 1;
         break;
  case 3: 
         operand1 = rand() % 100 + 1;
         operand2 = rand() % 100 + 1;
         break;
  default:
         operand1 = rand() % 101 - 100;
         operand2 = rand()% 101 - 100;
         break;
}

/* randomize arithmatic operation */
 switch(opValue){
    case 1:
          actualValue = operand1 + operand2;
          operat = "+";
          break;
    case 2:  
          actualValue = operand1 - operand2;
          operat = "-";
          break;
    case 3:
          actualValue = operand1 * operand2;
          operat = "*";
          break;
   default:
          actualValue = operand1 / operand2;
          operat = "/";
          break;
 }
/* print question */
  printf(" %d %s %d = ", operand1, operat, operand2);
  puts("");
  return actualValue;
}
bool answerQuestion(int value)
{
 int answer;
 /* prompt answer */
 printf("Enter Answer:");
 scanf("%d", &answer);
 return (answer == value) ? true : false;
}

void response(bool isCorrect)
{
 char* res;
 int randNumber = (rand() % 3) + 1;

/* determine random responses */
 if(isCorrect == true)
 {
  switch (randNumber)
  {
   case 1:
         res = "Nice!";
         break;
   case 2:
        res = "Good job!";
        break;
   default:
        res = "You're right!";
        break;
   }
 }else{
      switch (randNumber){
        case 1: 
              res = "Incorrect!";
              break;
        case 2:
              res = "Try again!";
              break;
        default:
              res = "That's incorrect!";
              break;
      }
  }
  /* print response */
printf("%s\n", res);
}


