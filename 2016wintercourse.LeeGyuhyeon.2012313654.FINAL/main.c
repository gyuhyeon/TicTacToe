/*
2017.1.11
Final Project : Tic Tac Toe Game
Made By Lee Gyuhyeon(2012313654)
*/

/*Libraries*/
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <math.h>

/*========Global variable declaration========*/

COORD consoleCoord = { 0, 0 };  //global varaible to use coordinate systems for drawing the game

								//game stats
int timesWon = 0;
int timesLost = 0;
int timesDraw = 0;

//game state to figure out how to translate mouse input coordinates
int isGameRunning = 0;

int board[9] = { 0,0,0,0,0,0,0,0,0 }; //game board
int turn = 1;    //what turn?(1~9)
int playerrole;  //what role for player(O/X)?

int computerChoice;  //what is the computer's choice?

					 /*--Bonus : Variables for Mouse Input--*/
HANDLE stdInputHandle;
DWORD oldConsoleSettings;
DWORD numReadCount, consoleSettings, dwordCounter;
INPUT_RECORD inputBuffer[128];


/*========User made function declaration========*/

/*-Internal UI & Drawing related-*/
void CursorSetPos(int x, int y);
void RenderFrame();
void RenderMainMenu();
void RenderSelect();
void RenderStats();
void RenderSettings();
void RenderBoard(int role);
void RenderTurn(int role, int pos);
void DrawO(int pos);
void DrawX(int pos);
void RenderEndGame(char msg[]);

/*-Gameplay related high layer functions-*/
void StartMainMenu();  //Start up the main menu that listens for user input
void SelectPlayer();   //Prompt for user to go first or second
void StartGame(int role);
void ShowStats();
void Settings();
void EndGame();

/*-Gameplay related low layer functions-*/
void PlayTurn(int role, int pos);
int IsActionValid(int board[], int pos);
int IsGameOver(int board[]);
void CopyBoard(int copy[], int original[]);
void InitBoard(int board[]);

/*-Artificial intelligence for Computer actions-*/
void ComputerAI();
int MiniMax(int board[], int turn, int role);
int MaxNumIndex(int array[], int index);
int MinNumIndex(int array[], int index);
int EvalScore(int board[], int role);

/*--Bonus : Functions for Mouse Input--*/
void InitConsoleMode();  //Required initialization before listening to standard input via mouse and keyboard
char UserInputListener();  //
void ErrorExit(LPSTR);
char KeyEventHandler(KEY_EVENT_RECORD);
char MouseEventHandler(MOUSE_EVENT_RECORD);
void ResizeEventHandler(WINDOW_BUFFER_SIZE_RECORD);


/*=====================MAIN FUNCTION(ENTRY POINT)=====================*/
int main(void)
{
	//No variables in main function needed(all modularized/global due to pointer usage ban)

	system("mode 81, 34"); //set console size
	system("color 0A");    //set color

						   //Required steps in order to properly run the program. 
	InitConsoleMode();

	//Start the main menu. This calls the game functions and all children. When it ends, the program ends.
	StartMainMenu();

	//Set cursor where the end message won't be obstructed in case additional end message comes up
	CursorSetPos(15, 15);

	//wait for user(Commented out. Making additional user input necessary is redundant)
	//system("PAUSE");

	// Restore input mode on exit.
	SetConsoleMode(stdInputHandle, oldConsoleSettings | ENABLE_EXTENDED_FLAGS);

	//goodbye
	return 0;
}


/*=====================Internal UI & Drawing related=====================*/

//Sets cursor(std output) position
void CursorSetPos(int x, int y) {
	consoleCoord.X = x;
	consoleCoord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), consoleCoord);
}

//Renders the basic game frame shared for all states
void RenderFrame() {
	int i, j; //for loop counters
			  //vertical frame
	for (i = 0; i < 31; ++i) {
		CursorSetPos(1, i);
		printf("|");
		CursorSetPos(79, i);
		printf("|");
	}
	//horizontal frame & credits
	CursorSetPos(1, 0);
	printf("===============================================================================");
	CursorSetPos(2, 30);
	printf("----------------------Made By Lee Gyuhyeon(2012313654)-----------------------");
	CursorSetPos(1, 31);
	printf("===============================================================================");
	CursorSetPos(18, 32);
	printf("Please do not change window size. Thank you :)");
}

//Renders main menu
void RenderMainMenu() {
	system("cls");  //clears screen
					/*
					SetConsoleMode will be used a lot in this program. This is required to make mouse click work after using system("cls") commands.
					To make things safe, all "Render" functions that has to do with stdout call this by default.
					*/
	SetConsoleMode(stdInputHandle, consoleSettings | ENABLE_EXTENDED_FLAGS);
	RenderFrame();
	CursorSetPos(27, 1);
	printf("########## Menu ##########");
	CursorSetPos(30, 3);
	printf("1 : Start Game\t¡á     <- Click Me!");
	CursorSetPos(30, 4);
	printf("2 : Stats\t\t¡á");
	CursorSetPos(30, 5);
	printf("3 : Settings\t¡á");
	CursorSetPos(30, 6);
	printf("4 : Quit\t\t¡á");
	CursorSetPos(20, 7);
	printf("Press key or click button > ");
}

//Renders select player menu
void RenderSelect() {
	system("cls");
	SetConsoleMode(stdInputHandle, consoleSettings | ENABLE_EXTENDED_FLAGS);
	RenderFrame();
	CursorSetPos(25, 1);
	printf("##### Please Select Player #####");
	CursorSetPos(30, 3);
	printf("1 : Player 1\t¡á");
	CursorSetPos(30, 4);
	printf("2 : Player 2\t¡á");
	CursorSetPos(20, 5);
	printf("Press key or click button > ");
}

//Renders statistics screen
void RenderStats() {
	system("cls");
	SetConsoleMode(stdInputHandle, consoleSettings | ENABLE_EXTENDED_FLAGS);
	RenderFrame();
	CursorSetPos(27, 1);
	printf("##### Game Statistics #####");
	CursorSetPos(9, 4);
	printf("Player won : %d times", timesWon);
	CursorSetPos(9, 6);
	printf("Computer won : %d times", timesLost);
	CursorSetPos(9, 8);
	printf("Draw : %d times", timesDraw);
	CursorSetPos(9, 10);
	printf("Press q or 1 to go back to main menu > ");

}

//Renders settings screen
void RenderSettings() {
	system("cls");
	SetConsoleMode(stdInputHandle, consoleSettings | ENABLE_EXTENDED_FLAGS);
	RenderFrame();
	CursorSetPos(20, 1);
	printf("##### Please Select Background Color #####");
	CursorSetPos(30, 3);
	printf("1 : Black\t\t¡á");
	CursorSetPos(30, 4);
	printf("2 : White\t\t¡á");
	CursorSetPos(30, 5);
	printf("3 : Black/Green\t¡á");
	CursorSetPos(30, 6);
	printf("4 : White/Red\t¡á");
	CursorSetPos(20, 7);
	printf("Press key or click button > ");
}

//Renders Tic tac toe game board
void RenderBoard(int role) {
	int i; //for loop counter
	system("cls");
	SetConsoleMode(stdInputHandle, consoleSettings | ENABLE_EXTENDED_FLAGS);
	RenderFrame();
	CursorSetPos(2, 1);
	printf("Player : %d", timesWon);
	CursorSetPos(2, 2);
	printf("Computer : %d", timesLost);
	CursorSetPos(2, 3);
	printf("Draw : %d", timesDraw);
	CursorSetPos(25, 1);
	if (role == 1) {
		printf("##### You're playing as %c #####", 'X');
	}
	else {
		printf("##### You're playing as %c #####", 'O');
	}
	//display current turn
	CursorSetPos(70, 1);
	printf("Turn : %d", turn);
	//game board exterior
	CursorSetPos(21, 7);
	printf("========================================");
	CursorSetPos(21, 14);
	printf("----------------------------------------");
	CursorSetPos(21, 21);
	printf("----------------------------------------");
	CursorSetPos(21, 28);
	printf("========================================");
	//game board interior
	for (i = 8; i < 28; ++i) {
		CursorSetPos(19, i);
		printf("||");
		CursorSetPos(34, i);
		printf("|");
		CursorSetPos(48, i);
		printf("|");
		CursorSetPos(61, i);
		printf("||");
	}
}

//Updates board by putting X and Os, and updating current turn display
void RenderTurn(int role, int pos) {
	if (role == 1) {
		DrawX(pos);
	}
	else {
		DrawO(pos);
	}
	if (turn < 10) {
		CursorSetPos(70, 1);
		printf("Turn : %d", turn);
	}
}

//Draw O
void DrawO(int pos) {
	int i; //for loop counter
	int x, y; //base point from where we'll be drawing
	x = 23 + ((pos - 1) % 3 * 14);
	y = 8 + ((pos - 1) / 3 * 7);
	CursorSetPos(x, y);
	printf("*********");
	CursorSetPos(x, y + 5);
	printf("*********");
	for (i = 0; i < 4; ++i) {
		CursorSetPos(x - 1, y + 1 + i);
		printf("*");
		CursorSetPos(x + 9, y + 1 + i);
		printf("*");
	}
}

//Draw X
void DrawX(int pos) {
	int i; //for loop counter
	int x, y; //base point from where we'll be drawing
	x = 23 + ((pos - 1) % 3 * 14);
	y = 9 + ((pos - 1) / 3 * 7);
	for (i = 0; i < 5; ++i) {
		CursorSetPos(x + i * 2, y + i);
		printf("+");
		CursorSetPos(x + (4 - i) * 2, y + i);
		printf("+");
	}

}

//Render(add) End Game messages(who won, how to exit to main menu, update stats)
void RenderEndGame(char msg[]) {
	CursorSetPos(35, 3);
	printf("%s", msg);    //who won?(msg comes from EndGame() function)
	CursorSetPos(30, 5);
	printf("Press q(r) to exit: ");
	CursorSetPos(2, 1);
	//update stats on screen
	printf("Player : %d", timesWon);
	CursorSetPos(2, 2);
	printf("Computer : %d", timesLost);
	CursorSetPos(2, 3);
	printf("Draw : %d", timesDraw);
}


/*=====================Gameplay related high layer functions=====================*/

//Main menu. Highest function except main, basically runs the game.
void StartMainMenu() {
	char userInput;  //for user input
	while (1) {
		//game global variables initialization back to default
		isGameRunning = 0;
		InitBoard(board);

		//draw the main menu
		RenderMainMenu();

		// Loop to read and handle the events(C cannot handle event listeners like C++)
		userInput = UserInputListener();
		while (userInput != '1' && userInput != '2' && userInput != '3' &&userInput != '4' && userInput != 'a' && userInput != 'b' && userInput != 'c' && userInput != 'd')
		{
			userInput = UserInputListener();
			//printf("%c", userInput);
		}

		//when while statement exits, userInput either has correct keyboard input(1~3) or correct mouse input(menu click translated as 'a'~'d')
		if (userInput == '1' || userInput == 'a') {
			SelectPlayer();
		}
		else if (userInput == '2' || userInput == 'b') {
			ShowStats();
		}
		else if (userInput == '3' || userInput == 'c') {
			Settings();
		}
		else if (userInput == '4' || userInput == 'd') {
			//end program
			return;
		}
	}
}

//Select Player
void SelectPlayer() {
	char userInput;
	RenderSelect();
	userInput = UserInputListener();
	while (userInput != '1' && userInput != '2' && userInput != 'a' && userInput != 'b')
	{
		userInput = UserInputListener();
	}
	if (userInput == '1' || userInput == 'a') {
		//Start game as player being 'X'
		StartGame(1);
	}
	else if (userInput == '2' || userInput == 'b') {
		//Start game as player being 'O'
		StartGame(2);
	}
}

//This is the game function(high level) that is called by StartMainMenu function. This is the game.
void StartGame(int role) {
	char userInput;  //variable for user input

					 /*initialize variables for game*/
	isGameRunning = 1;
	turn = 1;
	playerrole = role;

	//Draw the actual tic tac toe board
	RenderBoard(role);

	//infinite loop to simulate key event listener
	while (1) {

		//if the game has ended, end the game session.
		if (IsGameOver(board) != 0) {
			EndGame();
			return;
		}

		//if it is not our turn(our turn : role and turn are both even or odd. Computer's turn : role is even & turn is odd, or role is odd & turn is even.)
		//let computer play the turn.
		if ((abs(turn - role) % 2 == 1)) {
			ComputerAI();
			continue;     //without continue, game might ask for user turn after computer moved and won
		}

		//take user input
		userInput = UserInputListener();

		/*Hard coded for clarity and type conversion safety. Attempts to play turn at selected position(1~9). Mouse click gives 1~9 according to position.*/
		if (userInput == '1') {
			PlayTurn(role, 1);
		}
		else if (userInput == '2') {
			PlayTurn(role, 2);
		}
		else if (userInput == '3') {
			PlayTurn(role, 3);
		}
		else if (userInput == '4') {
			PlayTurn(role, 4);
		}
		else if (userInput == '5') {
			PlayTurn(role, 5);
		}
		else if (userInput == '6') {
			PlayTurn(role, 6);
		}
		else if (userInput == '7') {
			PlayTurn(role, 7);
		}
		else if (userInput == '8') {
			PlayTurn(role, 8);
		}
		else if (userInput == '9') {
			PlayTurn(role, 9);
		}
	}


}

//Show stats
void ShowStats() {
	char userInput;
	RenderStats();
	userInput = UserInputListener();
	while (userInput != 'q'&&userInput != '1'&&userInput != 'r')
	{
		userInput = UserInputListener();
	}
	return;
}

//Prompt settings
void Settings() {
	char userInput;
	RenderSettings();
	userInput = UserInputListener();
	while (userInput != '1' && userInput != '2' && userInput != '3' &&userInput != '4' && userInput != 'a' && userInput != 'b' && userInput != 'c' && userInput != 'd')
	{
		userInput = UserInputListener();
	}
	//when while statement exits, userInput either has correct keyboard input(1~3) or correct mouse input(menu click translated as 'a'~'c')

	if (userInput == '1' || userInput == 'a') {
		system("color 07");  //Black background + White letters
	}
	else if (userInput == '2' || userInput == 'b') {
		system("color 70");  //White background + Black letters
	}
	else if (userInput == '3' || userInput == 'c') {
		system("color 0A");  //Black + Green
	}
	else if (userInput == '4' || userInput == 'd') {
		system("color 74");  //White + Red
	}
}

//End the game
void EndGame() {
	char msg[100];
	char userInput;
	int result = IsGameOver(board);

	//player won
	if (result == playerrole) {
		timesWon += 1;
		strcpy_s(msg, sizeof(msg), "You won!");
	}
	//not draw(>0), so player lost if not won
	else if (result > 0) {
		timesLost += 1;
		strcpy_s(msg, sizeof(msg), "You lost!");
	}
	//draw
	else {
		timesDraw += 1;
		strcpy_s(msg, sizeof(msg), "Draw!");
	}
	//Render end game message
	RenderEndGame(msg);
	//end on q, 1, r
	userInput = UserInputListener();
	while (userInput != 'q' && userInput != '1' && userInput != 'r')
	{
		userInput = UserInputListener();
	}
	return;
}



/*=====================Gameplay related low layer functions=====================*/

//Play the turn with given role and board position(1~9)
void PlayTurn(int role, int pos) {
	if (IsActionValid(board, pos)) {
		//Caution : board is an array with 0~8 whereas position is 1~9. Need to subtract 1 from position.
		//put 1 or 2 depending on role on the board
		board[pos - 1] = role;
		//increase turn
		++turn;
		//call drawing function to apply to screen
		RenderTurn(role, pos);
	}
}

//Is the action valid?
int IsActionValid(int board[], int pos) {
	if (board[pos - 1] == 0) {
		return 1;
	}
	else {
		return 0;
	}
}

/*returns result of passed board. -1 : Draw, 0 : Undecided, 1 : Player 1 won, 2 : Player 2 won.*/
int IsGameOver(int board[]) {
	int i, j;  //for loop counter
	int piece = 0;
	int won = 0;

	//Horizontal winning check
	for (i = 0; i < 3; ++i) {
		piece = board[i * 3 + 0];
		won = piece;
		for (j = 1; j < 3; ++j) {
			if (piece == 0 || piece != board[i * 3 + j]) {
				won = 0;
				break;
			}
		}
		if (won != 0) {
			return won;
		}
	}

	//Vertical winning check
	for (j = 0; j < 3; ++j) {
		piece = board[j];
		won = piece;
		for (i = 1; i < 3; ++i) {
			if (piece == 0 || piece != board[i * 3 + j]) {
				won = 0;
				break;
			}
		}
		if (won != 0) {
			return won;
		}
	}

	//diagonal winning check
	piece = board[0];
	won = piece;
	for (i = 0; i < 3; ++i) {
		if (piece == 0 || piece != board[i * 3 + i]) {
			won = 0;
			break;
		}
	}
	if (won != 0) {
		return won;
	}

	//reverse diagonal winning check
	piece = board[2];
	won = piece;
	for (i = 1; i < 3; ++i) {
		if (piece == 0 || piece != board[i * 3 + (2 - i)]) {
			won = 0;
			break;
		}
	}
	if (won != 0) {
		return won;
	}

	//draw?
	if (won == 0 && board[0] != 0 && board[1] != 0 && board[2] != 0 && board[3] != 0 && board[4] != 0 && board[5] != 0 && board[6] != 0 && board[7] != 0 && board[8] != 0) {
		won = -1;
	}

	//either return 0(undecided) if it triggered nothing so far, or return -1(draw).
	return won;
}

//copies board to boardcopy. This is to make a temporary copy in order to modify board
void CopyBoard(int copy[], int original[]) {
	int i;
	for (i = 0; i < 9; ++i) {
		copy[i] = original[i];
	}
}

//initialize board to 0
void InitBoard(int board[]) {
	int i;
	for (i = 0; i < 9; ++i) {
		board[i] = 0;
	}
}


/*=====================Artificial intelligence for Computer actions=====================*/

//Highest function. Calls MiniMax with appropriate role for computer.
void ComputerAI() {
	if (playerrole == 1) {
		MiniMax(board, turn, 2);
		PlayTurn(2, computerChoice + 1);
	}
	else {
		MiniMax(board, turn, 1);
		PlayTurn(1, computerChoice + 1);
	}
}

//MiniMax function. Exhaustive search for best move.
int MiniMax(int board[], int turn, int role) {
	int i;   //for loop counter
	int maxnumindex;  //for storing index of maximum value in array
	int minnumindex;  //for storing index of minimum value in array
	int boardCopy[9];
	int scorelist[9] = { 0,0,0,0,0,0,0,0,0 };
	int s_i = -1;  // C lacks proper list structures. This is to keep track of top element position.
	int movelist[9] = { 0,0,0,0,0,0,0,0,0 };
	int m_i = -1;  // C lacks proper list structures. This is to keep track of top element position.
	CopyBoard(boardCopy, board);

	//return the score(recursive function endpoint) if the game is over at this point
	if (IsGameOver(boardCopy) != 0) {
		return EvalScore(boardCopy, ((playerrole == 1) ? 2 : 1));
	}

	/*This finds out what all the possible moves are, and what the scores for that are recursively.*/
	for (i = 0; i < 9; ++i) {
		//caution : pass position(1~9), not array position(0~8)

		//need to re-initialize. board contaminated.
		CopyBoard(boardCopy, board);

		if (IsActionValid(boardCopy, i + 1)) {
			//put 1 or 2 depending on role on the board
			boardCopy[i] = role;
			//pass the changed board to next MiniMax recursively.
			if (role == 1) {
				scorelist[++s_i] = MiniMax(boardCopy, turn + 1, 2);
			}
			else {
				scorelist[++s_i] = MiniMax(boardCopy, turn + 1, 1);
			}
			movelist[++m_i] = i;
		}
	}
	//active turn is minimax root player('role', or computer)
	if (abs(turn - ((playerrole == 1) ? 2 : 1)) % 2 == 0) {
		maxnumindex = MaxNumIndex(scorelist, s_i);
		computerChoice = movelist[maxnumindex];
		return scorelist[maxnumindex];
	}
	else {
		minnumindex = MinNumIndex(scorelist, s_i);
		return scorelist[minnumindex];
	}
}

//Returns maximum value index when given array and last index(size-1)
int MaxNumIndex(int arr[], int index) {
	int max_pos = 0;
	int i;
	for (i = 0; i <= index; ++i) {
		if (arr[i] > arr[max_pos]) {
			max_pos = i;
		}
	}
	return max_pos;
}

//Returns minimum value index when given array and last index(size-1)
int MinNumIndex(int arr[], int index) {
	int min_pos = 0;
	int i;
	for (i = 0; i <= index; ++i) {
		if (arr[i] < arr[min_pos]) {
			min_pos = i;
		}
	}
	return min_pos;
}

//Evaluates score(this is score used in MiniMax algorithm)
int EvalScore(int board[], int role) {
	int result = IsGameOver(board);
	if (role == result) {
		return 100;
	}
	else if (result>0) {
		return -100;
	}
	else if (result == -1) {
		return 0;
	}
}


/*=====================Bonus : Functions for Mouse Input=====================*/

//Initialize console input mode and settings
void InitConsoleMode() {
	// Get the standard input handle. 
	stdInputHandle = GetStdHandle(STD_INPUT_HANDLE);
	if (stdInputHandle == INVALID_HANDLE_VALUE)
		ErrorExit("GetStdHandle");
	// Save the current input mode, to be restored on exit. 
	if (!GetConsoleMode(stdInputHandle, &oldConsoleSettings))
		ErrorExit("GetConsoleMode");
	// Enable the window and mouse input events. 
	consoleSettings = ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT;
	consoleSettings &= ~ENABLE_QUICK_EDIT_MODE;
	if (!SetConsoleMode(stdInputHandle, consoleSettings | ENABLE_EXTENDED_FLAGS))
		ErrorExit("SetConsoleMode");
}

//This is a listener for user input. It returns input result in promised format. It ignores some input, and translates some to different input.
char UserInputListener() {

	//unnecessary redundancy, but might have to use if system("cls") and etc keep messing up mouse input
	//SetConsoleMode(stdInputHandle, consoleSettings | ENABLE_EXTENDED_FLAGS);

	char keypress; //pressed key

				   //stdInputHandle : input buffer handle 
				   //inputBuffer : buffer to read into 
				   //128 : size of read buffer 
				   //numReadCount : number of records read 
	if (!ReadConsoleInput(stdInputHandle, inputBuffer, 128, &numReadCount)) {
		ErrorExit("ReadConsoleInput");
	}

	//Send events to right handlers
	for (dwordCounter = 0; dwordCounter < numReadCount; dwordCounter++)
	{
		switch (inputBuffer[dwordCounter].EventType)
		{
		case KEY_EVENT: // keyboard input 
			keypress = KeyEventHandler(inputBuffer[dwordCounter].Event.KeyEvent);
			if (keypress != '0') {
				//Only react to keyUp rather than keyDown(allowing continuous pressing may lead to overflow/buggy/unexpected results)
				return keypress;
			}
			break;

		case MOUSE_EVENT: // mouse input 
			keypress = MouseEventHandler(inputBuffer[dwordCounter].Event.MouseEvent);
			if (keypress != '0') {
				return  keypress;
			}
			break;

		case WINDOW_BUFFER_SIZE_EVENT: // screen buffer resizing 
			ResizeEventHandler(inputBuffer[dwordCounter].Event.WindowBufferSizeEvent);
			break;

		case FOCUS_EVENT:  // disregard focus events 

			break;
		case MENU_EVENT:   // disregard menu events 

			break;

		default:
			ErrorExit("Unknown event. Only standard keyboard and mouse input are supported.");
			break;
		}
	}
	//'0' is going to be used to return that no valid input has been entered(0 key on keyboard isn't going to be used in program)
	return '0';
}

//Error exits
void ErrorExit(LPSTR lpszMessage) {
	fprintf(stderr, "%s\n", lpszMessage);

	// Restore input mode on exit.

	SetConsoleMode(stdInputHandle, oldConsoleSettings | ENABLE_EXTENDED_FLAGS);

	ExitProcess(0);
}

//Custom key event handler. It ignores all other input other than 1~9, q, r to prevent user error creating crashs, buffer overflow, etc.
char KeyEventHandler(KEY_EVENT_RECORD ker) {
	char c = ker.uChar.AsciiChar;
	if (ker.bKeyDown) {
		return '0';
	}
	else {
		if (c == '1' || c == '2' || c == '3' || c == '4' || c == '5' || c == '6' || c == '7' || c == '8' || c == '9' || c == 'q' || c == 'r')
			return ker.uChar.AsciiChar;
		else
			return '0';
	}
}

//Custom mouse event handler. Translates mouse click sections into pre-defined format. Robust temporary measure.
char MouseEventHandler(MOUSE_EVENT_RECORD mer) {
#ifndef MOUSE_HWHEELED
#define MOUSE_HWHEELED 0x0008
#endif
	//printf("Mouse event: ");

	switch (mer.dwEventFlags) {
	case 0:
		if (mer.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {

			/*a, b, c, d is coordinate cases for menu buttons*/
			if (mer.dwMousePosition.X >= 47 && mer.dwMousePosition.X <= 49 && mer.dwMousePosition.Y == 3) {
				return 'a';
			}
			else if (mer.dwMousePosition.X >= 47 && mer.dwMousePosition.X <= 49 && mer.dwMousePosition.Y == 4) {
				return 'b';
			}
			else if (mer.dwMousePosition.X >= 47 && mer.dwMousePosition.X <= 49 && mer.dwMousePosition.Y == 5) {
				return 'c';
			}
			else if (mer.dwMousePosition.X >= 47 && mer.dwMousePosition.X <= 49 && mer.dwMousePosition.Y == 6) {
				return 'd';
			}

			/*1,2,3,4,5,6,7,8,9 is for tic tac toe grid zones*/
			else if (isGameRunning == 0) {
				//If game is not running, do not return 1~9 input. Otherwise, clicking on that position will confuse main menu navigation.
				return '0';
			}
			else if (mer.dwMousePosition.X >= 20 && mer.dwMousePosition.X <= 33 && mer.dwMousePosition.Y >= 8 && mer.dwMousePosition.Y <= 14) {
				return '1';
			}
			else if (mer.dwMousePosition.X >= 34 && mer.dwMousePosition.X <= 46 && mer.dwMousePosition.Y >= 8 && mer.dwMousePosition.Y <= 14) {
				return '2';
			}
			else if (mer.dwMousePosition.X >= 48 && mer.dwMousePosition.X <= 60 && mer.dwMousePosition.Y >= 8 && mer.dwMousePosition.Y <= 14) {
				return '3';
			}
			else if (mer.dwMousePosition.X >= 20 && mer.dwMousePosition.X <= 33 && mer.dwMousePosition.Y >= 15 && mer.dwMousePosition.Y <= 20) {
				return '4';
			}
			else if (mer.dwMousePosition.X >= 34 && mer.dwMousePosition.X <= 46 && mer.dwMousePosition.Y >= 15 && mer.dwMousePosition.Y <= 20) {
				return '5';
			}
			else if (mer.dwMousePosition.X >= 48 && mer.dwMousePosition.X <= 60 && mer.dwMousePosition.Y >= 15 && mer.dwMousePosition.Y <= 20) {
				return '6';
			}
			else if (mer.dwMousePosition.X >= 20 && mer.dwMousePosition.X <= 33 && mer.dwMousePosition.Y >= 21 && mer.dwMousePosition.Y <= 27) {
				return '7';
			}
			else if (mer.dwMousePosition.X >= 34 && mer.dwMousePosition.X <= 46 && mer.dwMousePosition.Y >= 21 && mer.dwMousePosition.Y <= 27) {
				return '8';
			}
			else if (mer.dwMousePosition.X >= 48 && mer.dwMousePosition.X <= 60 && mer.dwMousePosition.Y >= 21 && mer.dwMousePosition.Y <= 27) {
				return '9';
			}
		}
		else if (mer.dwButtonState == RIGHTMOST_BUTTON_PRESSED) {
			//mouse right click, no need for this program.
		}
		else {
			//mouse click up. User experience will feel broken. Only compute click down events.
		}
		break;
	case DOUBLE_CLICK:
		//double click. No need for this program.
		break;
	case MOUSE_HWHEELED:
		//horizontal mouse wheel, no need for this program.
		break;
	case MOUSE_MOVED:
		//if mouse moved. No need for this program.
		break;
	case MOUSE_WHEELED:
		//mouse wheel. No need for this program.
		break;
	default:
		//other unknown events
		break;
	}
	return '0'; //default 'no valid input' response as '0'.
}

//Resize event handler, but not implemented. At the current state, user shouldn't sabotage by modifying window size.
void ResizeEventHandler(WINDOW_BUFFER_SIZE_RECORD wbsr) {
	//don't handle Resize event. It's too complicated to account for user intentionally sabotaging the program by modifying window size.
}