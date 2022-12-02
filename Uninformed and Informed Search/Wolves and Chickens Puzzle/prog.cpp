/***************************************************
 * CS331 
 * Assignment 1: Uninformed and Informed Search
 * Chia-Ling Hu
****************************************************/
#include <iostream>
#include <string.h>
#include <queue>
#include <vector>
#include <stack>
#include <fstream>
#include <sstream>


using namespace std;


struct State {
    int r_c;
    int r_w;
    int r_b;
    int l_c;
    int l_w;
    int l_b;
};


struct Node {
    int depth;
    Node* parent;
    State state;
    int h_val;
};

// compare function used in priority queue
struct cmp {
     bool operator() (Node*& a, Node*& b){
        return a->h_val >= b->h_val;
    }
};

// read the state from the start and goal file 
void get_file_state(State& initial, State& goal, char* init_fname, char* goal_fname){
        ifstream in_f, goal_f;
        string str;
        int arr[6];
        int i = 0;
        in_f.open(init_fname,ios::in);
        if (!in_f.is_open()){
            cout << "Error inital state file cannot be opened" << endl;
        }
        else if (in_f.is_open()){
            while(getline(in_f,str)){
                istringstream line(str);
                string value;
                while(getline(line,value,',')){
                    arr[i] = atoi(value.c_str());
                    i++;
                }
            }
        }
        initial.l_c = arr[0];
        initial.l_w = arr[1];
        initial.l_b = arr[2];
        initial.r_c = arr[3];
        initial.r_w = arr[4];
        initial.r_b = arr[5];
        in_f.close();
        i = 0;
        goal_f.open(goal_fname,ios::in);
        if (!goal_f.is_open()){
            cout << "Error goal state file cannot be opened" << endl;
        }
        else if (goal_f.is_open()) {     
            while(getline(goal_f,str)){
                istringstream line(str);
                string value;
                while(getline(line,value,',')){
                    arr[i] = atoi(value.c_str());
                    i++;
                }
            }
        }
        goal.l_c = arr[0];
        goal.l_w = arr[1];
        goal.l_b = arr[2];
        goal.r_c = arr[3];
        goal.r_w = arr[4];
        goal.r_b = arr[5];   
        goal_f.close();
}


// check if the two states are the same or not
bool issame(State s, State n){
    if((s.l_c==n.l_c)&&(s.l_w==n.l_w)&&(s.l_b==n.l_b)&&(s.r_c==n.r_c)&&(s.r_w==n.r_w)&&(s.r_b==n.r_b)){
        return true;
    }
    return false;
}

// check if the state is valid or not
bool isvalid(State s){
    if((s.l_c>=s.l_w || s.l_c == 0) && (s.r_c>=s.r_w || s.r_c == 0)){
        return true;
    }
    return false;
}

// print the state
void print_state(State s){
    cout << "Node with state: " << endl;
    cout << s.l_c << "," << s.l_w << "," << s.l_b << endl;
    cout << s.r_c << "," << s.r_w << "," << s.r_b << endl;
    cout << "----------------------" << endl;
}

// successor function 
bool succ(State s, State* newstate, int c, int w){
    // boat on the left side 
    if(s.l_b == 1){
        if(s.l_c>=c){
            newstate->l_c = s.l_c - c;
            newstate->r_c = s.r_c + c;
        }
        else {
            return false;
        }
        if(s.l_w>=w){
            newstate->l_w = s.l_w - w;
            newstate->r_w = s.r_w + w;
        }
        else {
            return false;
        }
        newstate->l_b = 0;
        newstate->r_b = 1;
        return true;
    }
    // boat on the right side 
    else if(s.r_b==1){
        if(s.r_c>=c){
            newstate->l_c = s.l_c + c;
            newstate->r_c = s.r_c - c;
        }
        else {
            return false;
        }
        if(s.r_w>=w){
            newstate->l_w = s.l_w + w;
            newstate->r_w = s.r_w - w;
        }
        else {
            return false;
        }
        newstate->r_b = 0;
        newstate->l_b = 1;
        return true;
    }
}

// check if the state is visited or not 
bool isvisit(vector<Node*> v_nodes, State s){
    vector<Node*>::iterator itr;
    for(itr=v_nodes.begin(); itr!=v_nodes.end();itr++){
        if(issame((*itr)->state,s)){
            return true;
        }
    }
    return false;
}

//  add node into queue
void bfs_add_ntoq(Node*& parent, Node*& newnd, State s, vector<Node*>& v_nodes, queue<Node*>& q){
    newnd = new Node;
    newnd->state = s;
    newnd->parent = parent;
    v_nodes.push_back(newnd);
    q.push(newnd);
}


// breadth first search 
Node* bfs(State init,State goal, int& expand){
    // queue to hold the nodes
    queue<Node*> q;
    // vector to store the visited nodes
    vector<Node*> v_nodes;
    Node* node = new Node;
    node->state = init;
    node->parent = NULL;
    Node* newnode;
    // add root node to queue
    q.push(node);
    // add root node to visited nodes
    v_nodes.push_back(node);
    expand = 0;
    while(!q.empty()){
        node = q.front();
        q.pop();
        // check if it is goal 
        if(issame(node->state,goal)){
            return node;
        }
        expand++;
        
        State newstate;
        // move one chicken
        if(succ(node->state,&newstate,1,0) && isvalid(newstate) && !isvisit(v_nodes,newstate)){
            bfs_add_ntoq(node,newnode,newstate,v_nodes,q);
        }
        // move two chicken
        if(succ(node->state,&newstate,2,0) && isvalid(newstate) && !isvisit(v_nodes,newstate)){
            bfs_add_ntoq(node,newnode,newstate,v_nodes,q);
        }
        // move one wolf
        if(succ(node->state,&newstate,0,1) && isvalid(newstate) && !isvisit(v_nodes,newstate)){
            bfs_add_ntoq(node,newnode,newstate,v_nodes,q);
        }
        // move one chicken one wolf
        if(succ(node->state,&newstate,1,1) && isvalid(newstate) && !isvisit(v_nodes,newstate)){
            bfs_add_ntoq(node,newnode,newstate,v_nodes,q);
        }
        // move two wolves
        if(succ(node->state,&newstate,0,2) && isvalid(newstate) && !isvisit(v_nodes,newstate)){
            bfs_add_ntoq(node,newnode,newstate,v_nodes,q);
        }
    }
    // no soultion found 
    return NULL;
} 

// add node into stack 
void dfs_add_ntost(Node*& parent, Node*& newnd,State s,vector<Node*>& v_nodes,stack<Node*>& st){
    newnd = new Node;
    newnd->state = s;
    newnd->parent = parent;
    v_nodes.push_back(newnd);
    st.push(newnd); 
}

// depth first search
Node* dfs(State init,State goal, int& expand){
    // stack to hold the nodes
    stack<Node*> st;
    // vector to store the visited nodes
    vector<Node*> v_nodes;
    Node* node = new Node;
    node->state = init;
    node->parent = NULL;
    Node* newnode;
    // add root node to stack
    st.push(node);
    // add root node to visited nodes
    v_nodes.push_back(node);
    expand = 0;
    while(!st.empty()){
        node = st.top();
        st.pop();
        // check if it is goal 
        if(issame(node->state,goal)){
            return node;
        }
        expand++;
        State newstate;
        // move one chicken
        if(succ(node->state,&newstate,1,0) && isvalid(newstate) && !isvisit(v_nodes,newstate)){
            dfs_add_ntost(node,newnode,newstate,v_nodes,st);
        }
        // move two chicken
        if(succ(node->state,&newstate,2,0) && isvalid(newstate) && !isvisit(v_nodes,newstate)){
            dfs_add_ntost(node,newnode,newstate,v_nodes,st);
        }
        // move one wolf
        if(succ(node->state,&newstate,0,1) && isvalid(newstate) && !isvisit(v_nodes,newstate)){
            dfs_add_ntost(node,newnode,newstate,v_nodes,st);
        }
        // move one chicken one wolf
        if(succ(node->state,&newstate,1,1) && isvalid(newstate) && !isvisit(v_nodes,newstate)){
            dfs_add_ntost(node,newnode,newstate,v_nodes,st);
        }
        // move two wolves
        if(succ(node->state,&newstate,0,2) && isvalid(newstate) && !isvisit(v_nodes,newstate)){
            dfs_add_ntost(node,newnode,newstate,v_nodes,st);
        }
    }
    // no solution found 
    return NULL;
}

// add node to stack
void iddfs_add_ntost(Node*& parent, Node*& newnd, State s, vector<Node*>& v_nodes,stack<Node*>& st){
    newnd = new Node;
    newnd->state = s;
    newnd->parent = parent;
    newnd->depth = parent->depth+1;
    v_nodes.push_back(newnd);
    st.push(newnd); 
}

// delete the node in the vector
void delete_node(vector<Node*> v_nodes){
    vector<Node*>::iterator itr;
    for(itr=v_nodes.begin(); itr!=v_nodes.end();itr++){
        delete *itr;
    }
}

// iterative deepening depth first search
Node* iddfs(State init,State goal, int& expand){
    // stack to hold the nodes
    stack<Node*> st;
    // vector to store the visited nodes
    vector<Node*> v_nodes;
    // the depth that increase in each iteraction 
    int it_depth = 0;
    // the limit max depth
    int max_depth = 600;
    bool found = false;
    
    while(1){
        Node* node = new Node;
        node->state = init;
        node->parent = NULL;
        node->depth = 0;
        Node* newnode;
        // add root node to stack
        st.push(node);
        // add root node to vistied nodes
        v_nodes.push_back(node);

        while(!st.empty()){
            node = st.top();
            st.pop();
            // check if it is goal 
            if(issame(node->state,goal)){
                found = true;
                return node;
            }
            
            expand++;
        // add successor node only if the node depth didn't exceed the current iteraction depth limit
            if(it_depth>=node->depth+1){
                State newstate;
            // move one chicken
                if(succ(node->state,&newstate,1,0) && isvalid(newstate) && !isvisit(v_nodes,newstate)){
                    iddfs_add_ntost(node,newnode,newstate,v_nodes,st);
                }
            // move two chicken
                if(succ(node->state,&newstate,2,0) && isvalid(newstate) && !isvisit(v_nodes,newstate)){
                    iddfs_add_ntost(node,newnode,newstate,v_nodes,st);
                }
            // move one wolf
                if(succ(node->state,&newstate,0,1) && isvalid(newstate) && !isvisit(v_nodes,newstate)){
                    iddfs_add_ntost(node,newnode,newstate,v_nodes,st);
                }
            // move one chicken one wolf
                if(succ(node->state,&newstate,1,1) && isvalid(newstate) && !isvisit(v_nodes,newstate)){
                    iddfs_add_ntost(node,newnode,newstate,v_nodes,st);
                }
            // move two wolves
                if(succ(node->state,&newstate,0,2) && isvalid(newstate) && !isvisit(v_nodes,newstate)){
                    iddfs_add_ntost(node,newnode,newstate,v_nodes,st);
                }
            }
        }
        // increase the depth after each iteraction
        it_depth++;
        // free the node and delete the node
        delete_node(v_nodes);
        v_nodes.clear();
        // the max depth is designed to be 600
        if(it_depth >= max_depth && found == false){
            // no soultion found 
            return NULL;
        }
    }
}


// add node to priority queue
void astar_add_ndtopq(Node*& parent, Node*& newnd,State s,State goal,vector<Node*>& v_nodes,priority_queue<Node*,vector<Node*>,cmp>& pq){
    newnd = new Node;
    newnd->state = s;
    newnd->parent = parent;
    newnd->h_val = (goal.l_c + goal.l_w + goal.l_b) - (s.l_c + s.l_w + s.l_b);
    v_nodes.push_back(newnd);
    pq.push(newnd);
}


// A star search 
Node* astar(State init, State goal, int& expand){
    // priority queue to hold the nodes
    priority_queue<Node*,vector<Node*>, cmp> pq;
    // vector to store the visited nodes
    vector<Node*> v_nodes;
    Node* node = new Node;
    node->state = init;
    node->parent = NULL;
    node->h_val = (goal.l_c + goal.l_w + goal.l_b) - (init.l_c + init.l_b + init.l_w);
    Node* newnode;
    // add root node to the priority queue
    pq.push(node);
    // add root node to the visited nodes
    v_nodes.push_back(node);
    expand = 0;

    while(!pq.empty()){
        node = pq.top();
        pq.pop();
        // check if it is goal 
        if(issame(node->state,goal)){
            return node;
        }
        expand ++;

        State newstate;
        // move one chicken 
        if(succ(node->state,&newstate,1,0) && isvalid(newstate) && !isvisit(v_nodes,newstate)){
            astar_add_ndtopq(node,newnode,newstate,goal,v_nodes,pq);
        }
        // move two chicken
        if(succ(node->state,&newstate,2,0) && isvalid(newstate) && !isvisit(v_nodes,newstate)){
            astar_add_ndtopq(node,newnode,newstate,goal,v_nodes,pq);
        }
        // move one wolf
        if(succ(node->state,&newstate,0,1) && isvalid(newstate) && !isvisit(v_nodes,newstate)){
            astar_add_ndtopq(node,newnode,newstate,goal,v_nodes,pq);
        }
        // move one chicken one wolf
        if(succ(node->state,&newstate,1,1) && isvalid(newstate) && !isvisit(v_nodes,newstate)){
            astar_add_ndtopq(node,newnode,newstate,goal,v_nodes,pq);
        }
        // move two wolves
        if(succ(node->state,&newstate,0,2) && isvalid(newstate) && !isvisit(v_nodes,newstate)){
            astar_add_ndtopq(node,newnode,newstate,goal,v_nodes,pq);
        }
    }
    // no solution found 
    return NULL;
}

// print the path from root node to solution node
void print_path(Node* solu,int& expand,char* out_fname){
        int count = 0;
        State temp;
        stack<State> stk; 
        ofstream out_f;
        out_f.open(out_fname,ios::out|ios::trunc);
        if(solu == NULL){
            cout << "No solution found in " << expand << " expand nodes" << endl;
            out_f << "No solution found in " << expand << " expand nodes" << endl;
        }
        else{
            cout << "Path from initial state to goal state" << endl;
            out_f << "Path from initial state to goal state" << endl;
            while(solu!=NULL){
                temp = solu->state;
                stk.push(temp);
                solu = solu->parent;                
            }
            while(!stk.empty()){
                print_state(stk.top());
                count ++;
                out_f << "Node with state: " << endl;
                out_f << stk.top().l_c << "," << stk.top().l_w << "," << stk.top().l_b << endl;
                out_f << stk.top().r_c << "," << stk.top().r_w << "," << stk.top().r_b << endl;
                out_f << "----------------------" << endl;
                stk.pop();
            }

            cout << "Total nodes expand: " << expand << endl;
            cout << "Number of nodes in solution path " << count << endl;
            out_f << "Total nodes expand: " << expand << endl;
            out_f << "Number of nodes in solution path " << count << endl;
        }
        out_f.close();
}


int main(int argc, char** argv){
        if(argc!=5){
            cout << "Too many or too few arguments" << endl;
            exit(1);
        }
        State init, goal;
        int expand;
        Node* solu;
        get_file_state(init,goal,argv[1],argv[2]);

        if(strcmp(argv[3],"bfs")==0){
            solu = bfs(init,goal,expand);
        }
        if(strcmp(argv[3],"dfs")==0){
            solu = dfs(init,goal,expand);
        }
        if(strcmp(argv[3],"iddfs")==0){
            solu = iddfs(init,goal,expand);
        }
        if(strcmp(argv[3],"astar")==0){
            solu = astar(init,goal,expand);
        }
        print_path(solu,expand,argv[4]);

    return 0;
}