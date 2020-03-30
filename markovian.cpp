#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <cstdlib>
#include <unistd.h>
using namespace std;

class Markovian{
    public:
        string word;
        vector<string> followers;
                       
        void displayFollowerWords(){
            cout << "Follower Words: ";
            for(int i = 0; i < followers.size(); i++){
                cout << followers.at(i) << ", ";
            }
            cout << endl;
        }

        void displayWords(){
            cout << "Base Word: " << word << endl;
        }

        Markovian(string baseWord, string followerWord){
            word = baseWord;
            followers.push_back(followerWord);
        }               
};

vector<Markovian> MarkovianChain;
string tweet = "";         

int getIndex(string word){
    for(int p = 0; p < MarkovianChain.size(); p++){
        if(MarkovianChain.at(p).word.compare(word) == 0){
            return p;
        }
    }
    return -1;
}

void formPairWords(string word, string follower){
    int index = getIndex(word);
    
    if(index == -1){
        MarkovianChain.push_back(Markovian(word, follower));
    }
    else{
        MarkovianChain.at(index).followers.push_back(follower);
    }
}

void lineParser(string line){

    string word = "";
    string follower = "";

    size_t wordSize;
    size_t followerLength;
    
    size_t startingWord = 0;
    size_t endingWord;
    size_t firstOfFollower;
    size_t lastOfFollower;

    while(startingWord != string::npos){
        endingWord = line.find(" ", startingWord);
        wordSize = endingWord - startingWord;
        
        while(wordSize == 0){
            startingWord++;
            endingWord = line.find(" ", startingWord);
            wordSize = endingWord - startingWord;
            
            if(startingWord >= line.length()){
                startingWord = line.length();
                break;
            }
        }
        
        word = line.substr(startingWord, wordSize); 

        if(endingWord != string::npos){
            firstOfFollower = endingWord + 1;
            lastOfFollower = line.find(" ", firstOfFollower);
            followerLength = lastOfFollower - firstOfFollower;
            
            while(followerLength == 0){
                firstOfFollower++;
                lastOfFollower = line.find(" ", firstOfFollower);
                followerLength = lastOfFollower - firstOfFollower;
                if(firstOfFollower >= line.length()){
                    firstOfFollower = line.length();
                    break;
                }
            }
            
            word = line.substr(startingWord, wordSize);
            follower = line.substr(firstOfFollower, followerLength); 
            
            startingWord = firstOfFollower;    
        }
        else{
            follower = "";
            startingWord = string::npos;
        }        
        formPairWords(word, follower);   
    }
}

bool textFileReader(string filename){
    string line;
    ifstream myfile(filename.c_str());
    if (myfile.is_open()){
        while(getline(myfile,line)){
            lineParser(line);
        }
        myfile.close();
        return true;
    }
    else{
        return false;
    }
}

int randomizeIndex(int value){
    return rand() % value;
}

void tweetFormationLogic(){
    
    int nextIndex = 0;
    int nextNumber = 0;
    tweet = "";
    string finalWord = "";
    int wordIndex = randomizeIndex(MarkovianChain.size()); 
    tweet = MarkovianChain.at(wordIndex).word; 
    finalWord = tweet;  
    
    bool tweetStatus = false;
   
    
    
    while(!tweetStatus){
        wordIndex = getIndex(finalWord);
        Markovian temp = MarkovianChain.at(wordIndex);
        
        nextNumber = temp.followers.size();
        nextIndex = randomizeIndex(nextNumber); 
                                                       
        finalWord = temp.followers.at(nextIndex);
    
        if(finalWord.length() == 0){    
            tweetStatus = true;
        }
        else{
            if(tweet.length() + finalWord.length() >= 279){ 
                tweetStatus = true;
            }
            else{
                tweet = tweet + " " + finalWord;   
            }
        }
    }
    cout << tweet << endl;
}



int main(int argc, char* argv[]){

    if(argc <= 1){
        
        cout << "Please Specify the filename to fetch tweets to proceed" << endl;
        cout<< "Quiting";
        return 0;
    }       
    string userParams(argv[1]);
    userParams = "MarkovianFileLocation/" + userParams + ".txt";
    
    if(textFileReader(userParams)){
        cout << "Generating Tweets: ";
        tweetFormationLogic();                
    }
    else{
        cout << "File "<< userParams << " was unavailable at specified location" << endl;
    }
    return 0;
}

