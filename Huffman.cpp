#include<bits/stdc++.h>
using namespace std;

// Define a class for Huffman Tree nodes
class HuffTree {
public:
    int val;          // Frequency of the character
    char letter;      // The character itself
    HuffTree* left;   // Pointer to the left child
    HuffTree* right;  // Pointer to the right child

    // Constructor to initialize the node
    HuffTree(int val, char c) {
        this->val = val;
        this->letter = c;
        this->left = nullptr;
        this->right = nullptr;
    }
};

// Function to calculate character frequencies in the input string
vector<pair<int, char>> FrequencyDect(string s) {
    map<char, int> mp;  // A map to store character frequencies

    // Iterate through the input string to calculate frequencies
    for (int i = 0; i < s.size(); i++) {
        mp[s[i]]++;
    }

    vector<pair<int, char>> v;  // A vector of pairs to store frequencies and characters

    // Iterate through the input string to create pairs of (frequency, character)
    for (int i = 0; i < s.size(); i++) {
        if (mp[s[i]] != 0) {
            v.push_back(make_pair(mp[s[i]], s[i]));  // Add the pair to the vector
            mp[s[i]] = 0;  // Mark the character as processed
        }
    }

    return v;  // Return the vector of character frequencies
}

// Custom comparator class for comparing Huffman Tree nodes by frequency
class Compare {
public:
    bool operator()(HuffTree* a, HuffTree* b) {
        return a->val > b->val;  // Compare nodes by frequency (lower frequency comes first)
    }
};

// Function to print the contents of a priority queue (for debugging)
void print(priority_queue<HuffTree*, vector<HuffTree*>, Compare> q) {
    while (!q.empty()) {
        cout << q.top()->letter << " " << q.top()->val << endl;
        q.pop();
    }
}

// Function to construct the Huffman Tree from character frequencies
HuffTree* makeTree(vector<pair<int, char>>& frq) {
    priority_queue<HuffTree*, vector<HuffTree*>, Compare> q;  // Priority queue to store Huffman Tree nodes

    // Initialize the priority queue with leaf nodes (character frequencies)
    for (int i = 0; i < frq.size(); i++) {
        q.push(new HuffTree(frq[i].first, frq[i].second));
    }

    // Build the Huffman Tree by merging nodes
    while (q.size() > 1) {
        HuffTree* left = q.top();  // Get the node with the lowest frequency
        q.pop();
        HuffTree* right = q.top();  // Get the next node with the lowest frequency
        q.pop();

        // Create a new node with a frequency equal to the sum of its children
        HuffTree* nd = new HuffTree(left->val + right->val, '~');
        nd->left = left;
        nd->right = right;

        q.push(nd);  // Add the new node back to the priority queue
    }

    return q.top();  // Return the root of the Huffman Tree
}

// Function to generate Huffman codes for characters and store them in a map
void getValues(HuffTree* root, string s, map<char, string>& vtr) {
    if (!root) {
        return;
    }
    if (root->letter != '~') {
        vtr[root->letter] = s;  // Store the Huffman code for the character
        return;
    } else {
        // Recursively traverse the tree to build codes
        getValues(root->left, s + '0', vtr);   // Append '0' when moving to the left child
        getValues(root->right, s + '1', vtr);  // Append '1' when moving to the right child
    }
}

// Function to decode a binary string using the Huffman Tree
string decode(HuffTree* tree, string s, int& i) {
    string str = "";
    while (i < s.size()) {
        if (tree->letter != '~') {
            str += tree->letter;  // Append the character to the result
            return str;
        } else if (s[i] == '0') {
            if (tree->left) {
                tree = tree->left;  // Move to the left child
                i++;
            } else {
                return str;
            }
        } else if (s[i] == '1') {
            if (tree->right) {
                tree = tree->right;  // Move to the right child
                i++;
            } else {
                return str;
            }
        }
    }
    return str;
}

int main() {
    string inputFileName = "input.txt"; // Specify the input file name
    string outputFileName = "output.txt"; // Specify the output file name

    ifstream inputFile(inputFileName);  // Open the input file for reading
    if (!inputFile) {
        cerr << "Error opening input file: " << inputFileName << endl;
        return 1;
    }

    string s;
    getline(inputFile, s); // Read the input string from the file

    inputFile.close(); // Close the input file

    vector<pair<int, char>> frq = FrequencyDect(s);  // Calculate character frequencies
    sort(frq.begin(), frq.end());  // Sort the frequencies

    map<char, string> vtr;  // Map to store character-Huffman code pairs
    HuffTree* tree = makeTree(frq);  // Construct the Huffman Tree

    getValues(tree, "", vtr);  // Generate Huffman codes

    string res = "";
    for (auto i : s) {
        res += vtr[i];  // Encode the input string into a binary string
    }

    string back = "";
    int i = 0;
    while (i < res.size()) {
        back += decode(tree, res, i);  // Decode the binary string
    }

    ofstream outputFile(outputFileName);  // Open the output file for writing
    if (!outputFile) {
        cerr << "Error opening output file: " << outputFileName << endl;
        return 1;
    }

    // Write the encoded and decoded data to the output file
    outputFile << "Encoded data:" << endl;
    outputFile << res << endl;
    outputFile << "Decoded data:" << endl;
    outputFile << back << endl;

    outputFile.close(); // Close the output file

    return 0;
}
