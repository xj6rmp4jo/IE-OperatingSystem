#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <mutex>
#include <ctime>
#include <chrono>
#include <unistd.h>
#include <sys/wait.h>
#include <sstream>

using namespace std;
using namespace chrono;

class piece { // used to slice file to K
  public : int start, mid, end;

  public : piece(int a, int b) {
    start = a, end = b, mid = 0;
  } // constructor
}; // class piece


void printMenu(char** filename, int& k, int& method);
void readFile(vector<int>& arr, ifstream& file);
char* StringTransToCharPointer(string s);
void sliceToK(vector<piece*>& pieces, int size, int k);
void bubbleSort(vector<int>& arr, int start, int end);
void mergeSort(vector<int>& arr, int left, int mid, int right);
void method_1(vector<int>& arr, int k);
void method_2(vector<int>& arr, int k);
void method_3(vector<int>& arr, int k);
void method_4(vector<int>& arr, int k);
void printTime(milliseconds duration);


int main() {
  while(true) {
    char* fileName;
    int k, method;
    ifstream file;

    do {
      printMenu(&fileName, k, method);
      file.open(fileName); // try to open file

     if (method == -1 || !file) cout << "Input error, please try again.\n";
    } while(method == -1 || !file);

    vector<int> arr;
    readFile(arr, file); // Read data from file

    if      (method == 1) method_1(arr, k);
    else if (method == 2) method_2(arr, k);
    else if (method == 4) method_4(arr, k);
  } // while(true)

} // main()


void printMenu(char** fileName, int& k, int& method) {
  string s;
  bool allIsDigit = true;

  cout << "Please enter the file name : ";
  getline(cin, s);
  s = s + ".txt";
  //cout << "s = " << s << "\n";
  *fileName = StringTransToCharPointer(s);
  //cout << "fileName = " << *fileName << "\n";

  cout << "Please enter the number of partitions : ";
  getline(cin, s);

  for (unsigned int a = 0; a < s.length(); a++) { // check input K
    if (!isdigit(s[a])) allIsDigit = false;
    else k = k * 10 + (s[a] - '0');
  } // for(a)

  cout << "Please enter the method No.(1, 2, 3, 4) : ";
  getline(cin, s);

	if (!isdigit(s[0]) || s.length() > 1) allIsDigit = false;
	else method = (s[0] - '0');

  if (!allIsDigit) method = -1; // means input error
} // printMenu()


void readFile(vector<int>& arr, ifstream& file) {
    int temp; // Read data and store it in a vector
    while(file >> temp) arr.push_back(temp);

    file.close(); // close file
} // readFile()


char* StringTransToCharPointer(string s) {
  char* fileName = new char[s.length() + 1]; // string transfer to char[]
  for (unsigned int a = 0; a < s.length(); a++) fileName[a] = s[a];
  fileName[s.length()] = '\0';
  return fileName;
} // StringTransToCharStar()


void sliceToK(vector<piece*>& pieces, int size, int k) {
  int pieceCapacity = size / k;
  unsigned int odd  = size % k;

  for (int a = 0 ; a < k ; a++) {
    int start = a       * pieceCapacity;
    int end   = (a + 1) * pieceCapacity - 1;

    if (end > size) end = size;

    piece* p = new piece(start, end);
    pieces.push_back(p);
  } // for(a)

  if (odd > 0) {
    for (unsigned int a = 0; a < pieces.size(); a++) {
      if      (a == 0)  pieces[0]->end = pieces[0]->end + 1;
      else if (a < odd) pieces[a]->start = pieces[a]->start + a,   pieces[a]->end = pieces[a]->end + a + 1;
      else              pieces[a]->start = pieces[a]->start + odd, pieces[a]->end = pieces[a]->end + odd;
    } // for(a)
  } // if()

} // sliceToK()


void bubbleSort(vector<int>& arr, int start, int end) {
  for (int i = start; i < end; i++)
    for (int j = i + 1; j <= end; j++)
      if (arr[i] > arr[j])
        swap(arr[i], arr[j]);
} // bubbleSort()


void mergeSort(vector<int>& arr, int left, int mid, int right) {
  vector<int> temp(right - left + 1); // Merge left and right subarrays

  int i = left, j = mid, k = 0;

  while (i < mid && j <= right) {
    if (arr[i] < arr[j]) temp[k++] = arr[i++];
    else                 temp[k++] = arr[j++];
  } // while()

  while (i <  mid)   temp[k++] = arr[i++];
  while (j <= right) temp[k++] = arr[j++];

  k = 0; // Copy back to the original vector
  for (i = left; i <= right; i++) arr[i] = temp[k++];
} // mergeSort()


void method_1(vector<int>& arr, int k) {
  auto start = high_resolution_clock::now(); // Start timing

  bubbleSort(arr, 0, arr.size() - 1); // Execute bubbleSort

  auto end = high_resolution_clock::now(); // Stop timing
  auto duration = duration_cast<milliseconds>(end - start); // Calculate execution time in milliseconds

  //for (int i = 0; i < arr.size(); i++) cout << arr[i] << "\n";
  printTime(duration);
} // method_1()


void method_2(vector<int>& arr, int k) {
/*
  vector<piece*> pieces; // Split the array into pieces, where piece is a struct
  sliceToK(pieces, arr.size(), k); // containing the start and end positions of the block

  auto start = high_resolution_clock::now(); // Start timing

  pid_t pid = fork(); // fork a process

  if (pid == 0) { // child process
    for (int a = 0; a < k; a++) // Perform bubble sort on each data segment
      bubbleSort(arr, pieces[a]->start, pieces[a]->end);
    exit(0); // End child process
  } // if()
  else { // parent process
    waitpid(pid, NULL, 0); // Wait for child process to finish

    mutex mtx;
    mergeSort(arr, 0, arr.size() - 1, mtx); // Perform merge sort
  } // else

  auto end = high_resolution_clock::now(); // Stop timing
  auto duration = duration_cast<milliseconds>(end - start); // Calculate execution time in milliseconds

  //for (int i = 0; i < arr.size(); i++) cout << arr[i] << "\n";
  printTime(duration);
  */
} // method_1()


void method_3(vector<int>& arr, int k) {/*
  vector<piece*> pieces; // Split the array into pieces, where piece is a struct
  sliceToK(pieces, arr.size(), k); // containing the start and end positions of the block

  auto start = high_resolution_clock::now(); // Start timing

  pid_t pid;
  int status;

  for(int a = 0; a < k; a++) { // Fork k child processes to perform bubble sort
    pid = fork();

    if(pid == 0) { // Child process
      bubbleSort(arr, pieces[a]->start, pieces[a]->end);
      exit(0);
    } // if()
  } // for(a)

  while(wait(&status) > 0) ; // Wait for all child processes to finish

  for(int i = 0; i < k - 1; i++) { // Fork k - 1 child processes to perform merge sort
    pid = fork();

    if(pid == 0) { // Child process
      int left_piece = i;
      int right_piece = i + 1;
      vector<int> merged(pieces[left_piece].size() + pieces[right_piece].size());
      merge(pieces[left_piece].begin(), pieces[left_piece].end(), pieces[right_piece].begin(), pieces[right_piece].end(), merged.begin());
      pieces[right_piece] = merged;
      exit(0);
    } // if()
  } // for(i)

    // Wait for all child processes to finish
    while(wait(&status) > 0) ;

    // The final sorted array is stored in pieces[k-1]
    vector<int> sorted_arr = pieces[k-1];
    */
} // method_3


void method_4(vector<int>& arr, int k) {
  vector<piece*> pieces; // Split the array into pieces, where piece is a struct
  sliceToK(pieces, arr.size(), k); // containing the start and end positions of the block

  auto start = high_resolution_clock::now(); // Start timing

  vector<thread> thread_bubble(k); // Create threads and execute bubbleSort
  for (int a = 0; a < k; a++)
    thread_bubble[a] = thread(bubbleSort, ref(arr), pieces[a]->start, pieces[a]->end);
  for (int a = 0; a < k; a++) thread_bubble[a].join(); // Wait for all threads to finish


  while (pieces.size() > 1) { // Create threads and execute mergeSort
    int a = 0;

    while(a + 1 < pieces.size()) {
      pieces[a]->mid = pieces[a + 1]->start;
      pieces[a]->end = pieces[a + 1]->end;
      pieces.erase(pieces.begin() + ++a);
    } // while()

    vector<thread> thread_merge(pieces.size());
    for (int b = 0; b < pieces.size(); b++)
      thread_merge[b] = thread(mergeSort, ref(arr), pieces[b]->start, pieces[b]->mid, pieces[b]->end);
    for (int b = 0; b < pieces.size(); b++) thread_merge[b].join(); // Wait for all threads to finish
  } // while()

  auto end = high_resolution_clock::now(); // Stop timing
  auto duration = duration_cast<milliseconds>(end - start); // Calculate execution time in milliseconds

  for (int i = 0; i < arr.size(); i++) cout << arr[i] << "\n";
  printTime(duration);
} // method_4()


void printTime(milliseconds duration) {
  cout << "\nCPU Time : " << duration.count()<< "ms\n";

  stringstream result, offset; // Below is the code for handling Output Time
  time_t t = time(nullptr); // Get the current time
  tm local_tm = *localtime(&t);

  char buffer[80]; // Format the time string
  strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &local_tm);
  result << buffer;

  time_t utc = time(nullptr); // Get the local time zone
  tm utc_tm = *gmtime(&utc);
  int offset_hour = (difftime(mktime(&local_tm), mktime(&utc_tm)) / 3600);
  if (offset_hour >= 0) offset << "+";
  if (offset_hour < 10) offset << "0";
  offset << offset_hour << ":00";
  result << offset.str();

  cout << result.str() << "\n\n--------------------\n\n";
} // printTime(()