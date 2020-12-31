// Ashutosh Kumar Singh
// 19CS30008

#include <bits/stdc++.h>
using namespace std;

#define MAX 10000

/**********************************************************/
/************** Stack Datatype and Operations *************/
/**********************************************************/

// stack structure declaration
typedef struct stackADT {
    char opr[MAX];
    int top;
} Stack;

// initialize stack
void initStack (Stack *stack)
{
    stack->top = -1;
}

// check whether stack is empty
int isEmptyStack (Stack *stack)
{
    return (stack->top == -1);
}

// check whether stack is full
int isFullStack (Stack *stack)
{
    return (stack->top == MAX-1);
}

// push an element into stack
void push (Stack *stack, char x)
{
    if (isFullStack(stack))
        printf("Error: Stack is Full!\n");
    else
        stack->opr[++(stack->top)] = x;
}

// pop an element from stack
char pop (Stack *stack)
{
    char x;
    if (isEmptyStack(stack))
        printf("Error: Stack is Empty!\n");
    else
        x = stack->opr[(stack->top)--];
    return x;
}

/**********************************************************/


/**********************************************************/
/*********** Binary Tree Datatype and Operations **********/
/**********************************************************/

// binary tree structure declaration
typedef struct binTree {
    char element;
    struct binTree *leftChild;
    struct binTree *rightChild;
} BT;

// creating a node in binary tree
BT *createNode (char el)
{
    BT *newNode;
    if ( (newNode=(BT *)malloc(sizeof(BT))) == NULL )
        printf("Error: Malloc Error in Creating Node!\n");
    else {
        newNode->element = el;
        newNode->leftChild = NULL;
        newNode->rightChild = NULL;
    }
    return (newNode);
}

// creating an exact replica of the the binary tree
BT *duplicate(BT *orig)
{
    BT *dup = NULL;
    if(orig != NULL) {
        dup = createNode(orig->element);
        dup->leftChild = duplicate(orig->leftChild);
        dup->rightChild = duplicate(orig->rightChild);
    }
    return (dup);
}

/**********************************************************/


/**********************************************************/
/******************** Utility Functions *******************/
/**********************************************************/

// structure holding propositional values
typedef struct PropVal {
    char prop;
    int val; // '0' for False and '1' for True
} PV;

// scan every propositional values (one by one) from user as input
vector<PV> scanPropValueInput()
{
    unsigned int noProp, i;
    
    printf("Enter Total Number of Propositions: ");
    scanf("%u",&noProp);
    
    vector<PV> pvarr(noProp);
    
    for (i = 0; i < noProp; i++) {
        printf("Enter Proposition [%u] (Format: Name <SPACE> Value): ", i+1);
        scanf(" %c %d", &pvarr[i].prop, &pvarr[i].val);
    }

    return pvarr;
}

// determines whether p is a proposition including 0 or 1
int isProposition (char p)
{
    return ( ((p >= 'a') && (p <= 'z')) || ((p >= 'A') && (p <= 'Z')) || (p == '0') || (p == '1') );
}

// printing the validity and satisfiability flags
void printResult (int valid, int sat)
{
    printf("\nThe Given Formula is: < ");
    valid ? printf("VALID") : printf("INVALID");
    printf(" + ");
    sat ? printf("SATISFIABLE") : printf("UNSATISFIABLE");
    printf(" >\n\n");
}

//display postfix form of propositional formula (from internally represented string)
void displayPfForm(char *pfForm)
{
    int i;
    
    printf("Postfix Representation of Formula:");
    for(i = 0; pfForm[i] != '\0'; i++){
        if ( pfForm[i] == '-' )
            printf(" ->");
        else if (pfForm[i] == '~')
            printf(" <->");
        else
            printf(" %c", pfForm[i]);
    }
    printf("\n");
}

// count number of characters in the formula representing only propositions and operators
int noOfIdsInFormula (char *formula)
{
    int i, len = strlen(formula), count = 0;
    for(i = 0; i < len; i++){
        if ( (formula[i] != '(') && (formula[i] != ')') && (formula[i] != ' ') && (formula[i] != '\t') )
            count++;
    }
    return count;
}

// pre-defined priority of in-stack element
int inStackPriority (char op)
{
    switch(op){
        case '!': return 3; break;
        
        case '&':
        case '|': return 2; break;
        
        case '~':
        case '-': return 1; break;
        
        case '(': return 0; break;
        
        default : return -1; break;
    }
}

// pre-defined priority of in-coming element
int inComingPriority (char op)
{
    switch(op){
        case '!': return 4; break;
        
        case '&':
        case '|': return 2; break;
        
        case '~':
        case '-': return 1; break;
        
        case '(': return 4; break;
        
        default : return -1; break;
    }
}

// generate postfix formula from the given input formula
char *genPostFixFormula(char *formula)
{
    int noOfIds = noOfIdsInFormula(formula), i, len = strlen(formula), k;
    char *pf = (char *)malloc((noOfIds+1) * sizeof(char));
    char out;
    Stack *stack = (Stack *)malloc(sizeof(Stack));
    initStack(stack); push(stack,'#');
    
    for (i = 0, k = 0; i < len; i++){
        if ( (formula[i] != ' ') && (formula[i] != '\t') ){
            if ( isProposition(formula[i]) )
                pf[k++] = formula[i];
            else if (formula[i] == ')') {
                while ( (out = pop(stack)) != '(')
                    pf[k++] = out;
            }
            else {
                while ( inStackPriority(out = pop(stack)) >= inComingPriority(formula[i]) )
                    pf[k++] = out;
                push(stack, out);
                push(stack, formula[i]);
            }
        }
    }
    while( (out = pop(stack)) != '#' )
        pf[k++] = out;
    pf[k] = '\0';
    
    return pf;
}

/**********************************************************/


/**********************************************************/
/****************** YOUR CODE STARTS HERE *****************/
/**********************************************************/

// expression tree formation from postfix formula string
BT *ETF (char *pfForm, int start, int end)
{
    BT *et;
    
    // base case when we have a single proposition
    if(start == end)
        et = createNode(pfForm[start]);
    else if(pfForm[end] == '!')
    {
        et = createNode('!');
        et -> rightChild = ETF(pfForm, start, end - 1);
    }
    else
    {
        et = createNode(pfForm[end]);
        int i, cnt = 0;
        // finding the index when count of propositions becomes greater than coutn of operators
        for(i = end - 1; i >= start; i--)
        {
            if(pfForm[i] == '!')
                continue;

            if(isProposition(pfForm[i]))
                cnt--;
            else
                cnt++;
            if(cnt == -1)
                break;
        }
        et -> leftChild = ETF(pfForm, start, i - 1); // recurse to create the left subtree
        et -> rightChild = ETF(pfForm, i, end - 1); // recurse to create the right subtree
    }
    
    return et;
}

// storing the expresion tree in a string using inorder traversal
void ETP (BT *et)
{
    if(et == NULL)
        return;

    // to make the output cleaner, we do not print brackets when we encounter a proposition or a not(!) operator

    if(et -> element == '!')
    {
        ETP(et -> leftChild);
        printf("%c ", et -> element);
        ETP(et -> rightChild);
        return;
    }
    if(isProposition(et -> element))
    {
        printf("%c ", et -> element);
        return;
    }
    printf("( ");
    ETP(et -> leftChild);

    if(et -> element == '-')
        printf("-> ");
    else if(et -> element == '~')
        printf("<-> ");
    else
        printf("%c ", et -> element);

    ETP(et -> rightChild);
    printf(") ");
}

// evaluate the formula from the expression tree from the proposition values provided in varr vector
int EVAL (BT *et, vector<PV>& pvarr)
{
    // for the base case, when we encounter a proposition, find its value from the passed vector
    if(isProposition(et -> element))
    {
        int i;
        for(i = 0; i < (int)pvarr.size(); i++)
        {
            if(pvarr[i].prop == et -> element)
                return pvarr[i].val;
        }
        if(i == (int)pvarr.size())
        {
            printf("Value of proposition not found.\n\n");
            exit(0);
        }
    }

    // if we get an operator, we evaluate the left and right subtrees and then evaluate the result accordingly
    else if(et -> element == '!')
        return ((EVAL(et -> rightChild, pvarr) == 1) ? 0 : 1);
    else if(et -> element == '&')
        return (EVAL(et -> leftChild, pvarr) && EVAL(et -> rightChild, pvarr));
    else if(et -> element == '|')
        return (EVAL(et -> leftChild, pvarr) || EVAL(et -> rightChild, pvarr));
    else if(et -> element == '-')
        return (((EVAL(et -> leftChild, pvarr) == 1) && (EVAL(et -> rightChild, pvarr) == 0)) ? 0 : 1);
    else if(et -> element == '~')
        return ((EVAL(et -> leftChild, pvarr) == 1 && EVAL(et -> rightChild, pvarr) == 1) || (EVAL(et -> leftChild, pvarr) == 0 && EVAL(et -> rightChild, pvarr) == 0));
}

// convert expression tree to IFF expression tree
BT *IFF (BT *et)
{
    // base cases
    if(et == NULL)
        return et;
    else if(isProposition(et -> element))
        return et;

    // (p -> q) is replaced by (!p | q)
    if(et -> element == '-')
    {
        et -> element = '|';
        BT *etLeft = et -> leftChild;
        et -> leftChild = createNode('!');
        et -> leftChild -> rightChild = IFF(etLeft);

        et -> rightChild = IFF(et -> rightChild);
    }
    // (p <-> q) is replaced by ((p -> q) & (q -> p))
    else if(et -> element == '~')
    {
        et -> element = '&';
        BT *etLeft = et -> leftChild;
        BT *etRight = et -> rightChild;
        et -> leftChild = createNode('-');
        et -> rightChild = createNode('-');

        et -> leftChild -> leftChild = etLeft;
        et -> leftChild -> rightChild = etRight;

        et -> rightChild -> leftChild = duplicate(etRight);
        et -> rightChild -> rightChild = duplicate(etLeft);

        et -> leftChild = IFF(et -> leftChild);
        et -> rightChild = IFF(et -> rightChild);
    }
    // if any other symbol is encountered just recurse on the left and right subtrees
    else
    {
        et -> leftChild = IFF(et -> leftChild);
        et -> rightChild = IFF(et -> rightChild);
    }
    
    return et;
}

// convert IFF expression tree to NNF expression tree
BT *NNF (BT *etI)
{
    // base cases
    if(etI == NULL)
        return etI;
    if(isProposition(etI -> element))
        return etI;

    // pushing the not operator inside using De Morgan's Laws
    if(etI -> element == '!')
    {
        // !(!p) is equivalent to p
        if(etI -> rightChild -> element == '!')
        	etI = NNF(etI -> rightChild -> rightChild);

        // !(p & q) is equivalent to (!p | !q)
        else if(etI -> rightChild -> element == '&')
        {
            BT *p = etI -> rightChild -> leftChild;
            BT *q = etI -> rightChild -> rightChild;
            etI -> element = '|';
            etI -> leftChild = createNode('!');
            etI -> rightChild = createNode('!');
            etI -> leftChild -> rightChild = p;
            etI -> rightChild -> rightChild = q;

            etI -> leftChild = NNF(etI -> leftChild);
            etI -> rightChild = NNF(etI -> rightChild);
        }

        // !(p | q) is equivalent to (!p & !q)
        else if(etI -> rightChild -> element == '|')
        {
            BT *p = etI -> rightChild -> leftChild;
            BT *q = etI -> rightChild -> rightChild;
            etI -> element = '&';
            etI -> leftChild = createNode('!');
            etI -> rightChild = createNode('!');
            etI -> leftChild -> rightChild = p;
            etI -> rightChild -> rightChild = q;

            etI -> leftChild = NNF(etI -> leftChild);
            etI -> rightChild = NNF(etI -> rightChild);
        }
        else
        {
            etI -> leftChild = NNF(etI -> leftChild);
            etI -> rightChild = NNF(etI -> rightChild);
        }
    }

    // for any other operator
    etI -> leftChild = NNF(etI -> leftChild);
    etI -> rightChild = NNF(etI -> rightChild);

    return etI;
}

// convert NNF expression tree to CNF expression tree
BT *CNF (BT *etN)
{
    // base cases
    if(etN == NULL)
        return etN;
    if(isProposition(etN -> element))
        return etN;

    if(etN -> element == '|' && etN -> leftChild -> element != '&' && etN -> rightChild -> element != '&')
    {
    	etN -> leftChild = CNF(etN -> leftChild);
        etN -> rightChild = CNF(etN -> rightChild);
    }

    // ((q1 & q2) | p) is converted to ((q1 | p) & (q2 | p))
    if(etN -> element == '|' && etN -> leftChild -> element == '&')
    {
    	BT *p = etN -> rightChild;
        BT *q1 = etN -> leftChild -> leftChild;
        BT *q2 = etN -> leftChild -> rightChild;

        etN -> element = '&';
        etN -> leftChild = createNode('|');
        etN -> rightChild = createNode('|');

        etN -> leftChild -> leftChild = q1;
        etN -> leftChild -> rightChild = duplicate(p);

        etN -> rightChild -> leftChild = q2;
        etN -> rightChild -> rightChild = duplicate(p);

        etN -> leftChild = CNF(etN -> leftChild);
        etN -> rightChild = CNF(etN -> rightChild);
    }

    // (p | (q1 & q2)) is converted to ((p | q1) & (p | q2))
    if(etN -> element == '|' && etN -> rightChild -> element == '&')
    {
    	BT *p = etN -> leftChild;
        BT *q1 = etN -> rightChild -> leftChild;
        BT *q2 = etN -> rightChild -> rightChild;

        etN -> element = '&';
        etN -> leftChild = createNode('|');
        etN -> rightChild = createNode('|');

        etN -> leftChild -> leftChild = duplicate(p);
        etN -> leftChild -> rightChild = q1;

        etN -> rightChild -> leftChild = duplicate(p);
        etN -> rightChild -> rightChild = q2;

        etN -> leftChild = CNF(etN -> leftChild);
        etN -> rightChild = CNF(etN -> rightChild);
    }

    // for any other operator
    etN -> leftChild = CNF(etN -> leftChild);
    etN -> rightChild = CNF(etN -> rightChild);
    
    return etN;
}


// convert NNF expression tree to DNF expression tree
BT *DNF (BT *etN)
{
    // base cases
    if(etN == NULL)
        return etN;
    if(isProposition(etN -> element))
        return etN;

    if(etN -> element == '&' && etN -> leftChild -> element != '|' && etN -> rightChild -> element != '|')
    {
    	etN -> leftChild = DNF(etN -> leftChild);
        etN -> rightChild = DNF(etN -> rightChild);
    }

    // ((q1 | q2) & p) is converted to ((q1 & p) | (q2 & p))
    if(etN -> element == '&' && etN -> leftChild -> element == '|')
    {
    	BT *p = etN -> rightChild;
        BT *q1 = etN -> leftChild -> leftChild;
        BT *q2 = etN -> leftChild -> rightChild;

        etN -> element = '|';
        etN -> leftChild = createNode('&');
        etN -> rightChild = createNode('&');

        etN -> leftChild -> leftChild = q1;
        etN -> leftChild -> rightChild = duplicate(p);

        etN -> rightChild -> leftChild = q2;
        etN -> rightChild -> rightChild = duplicate(p);

        etN -> leftChild = DNF(etN -> leftChild);
        etN -> rightChild = DNF(etN -> rightChild);
    }

    // (p & (q1 | q2)) is converted to ((p & q1) | (p & q2))
    if(etN -> element == '&' && etN -> rightChild -> element == '|')
    {
    	BT *p = etN -> leftChild;
        BT *q1 = etN -> rightChild -> leftChild;
        BT *q2 = etN -> rightChild -> rightChild;

        etN -> element = '|';
        etN -> leftChild = createNode('&');
        etN -> rightChild = createNode('&');

        etN -> leftChild -> leftChild = duplicate(p);
        etN -> leftChild -> rightChild = q1;

        etN -> rightChild -> leftChild = duplicate(p);
        etN -> rightChild -> rightChild = q2;

        etN -> leftChild = DNF(etN -> leftChild);
        etN -> rightChild = DNF(etN -> rightChild);
    }

    // for any other operator
    etN -> leftChild = DNF(etN -> leftChild);
    etN -> rightChild = DNF(etN -> rightChild);
    
    return etN;
}

// exhaustive search for checking validity / satisfiability
void CHECK (BT *et)
{
    int valid = 1, sat = 0;
    int n, i, result;
    long long mask;

    printf("Enter Number of Propositions: ");
    scanf("%d", &n);

    vector<PV> pvarr(n);
    char ch;

    printf("Enter Proposition Names (<SPACE> Separated): ");
    for(i = 0; i < n; i++)
        scanf("%c %c", &ch, &pvarr[i].prop);

    printf("Evaluations of the Formula:\n");

    // here we have to generate all binary strings of length n
    // so we consider integers from 0 to (2 ^ n - 1)
    // the n - length binary representation of each integer corresponds to one string
    for(mask = 0; mask < (1LL << n); mask++)
    {
        for(i = 0; i < n; i++)
            pvarr[n - i - 1].val = ((mask >> i) & 1LL);
        printf("    {");
        for(i = 0; i < n; i++)
            printf("  (%c = %d)", pvarr[i].prop, pvarr[i].val);
        printf("  }  :  ");
        
        result = EVAL(et, pvarr);
        printf("%d\n", result);
        if(result)
            sat = 1;
        else
            valid = 0;
    }
    
    printResult(valid, sat);
}

/**********************************************************/
/******************* YOUR CODE ENDS HERE ******************/
/**********************************************************/


// main routine
int main ()
{
    char formula[MAX], *pfForm;
    int len, i;
    
    BT *et, *etI, *etN, *etDup, *etC, *etD;
    int *varr;
    vector<PV> pvarr;
    int result;
    
    // scan propositional formula from user input
    printf("\nEnter Propositional Logic Formula: ");
    scanf("%[^\n]", formula);
    
    // internal formula string with operators as, AND (&), OR (|), NOT (!), IMPLY (-) and IFF (~)
    len = strlen(formula);
    for(i = 0; i < len; i++){
        if(formula[i] == '<'){ // denoting iff operator (<->) using ~
            formula[i] = ' ';
            formula[i+1] = '~';
        }
        if(formula[i] == '>'){ // denoting imply operator (->) using -
            formula[i] = ' ';
        }
    }
    
    // postfix form generation from represented formula string
    pfForm = genPostFixFormula(formula);
    
    // display postfix form of the internally represented formula
    displayPfForm(pfForm);
    
    // internal postfix formula string with operators as, AND (&), OR (|), NOT (!), IMPLY (-) and IFF (~)
    printf("\n++++ PostFix Format of the Propositional Formula ++++\n('-' used for '->' and '~' used for '<->')\n");
    printf("YOUR INPUT STRING: %s\n", pfForm);
    
    
    
    /**********************************************************/
    /********** YOUR CODE ENABLES THE FOLLOWING PARTS *********/
    /**********************************************************/
    
    printf("\n++++ Expression Tree Generation ++++");
    et = ETF(pfForm, 0, strlen(pfForm)-1);
    printf("\nOriginal Formula (from Expression Tree): ");
    ETP(et);
    printf("\n");

    //CHECK (et);
    
    
    printf("\n++++ Expression Tree Evaluation ++++\n");
    pvarr = scanPropValueInput();
    result = EVAL(et, pvarr);
    printf("\nThe Formula is Evaluated as: ");
    (result) ? printf("True\n") : printf("False\n");
    

    printf("\n++++ IFF Expression Tree Conversion ++++");
    etI = IFF(et);
    printf("\nFormula in Implication Free Form (IFF from Expression Tree):\n");
    ETP(etI);
    printf("\n");
    
    
    printf("\n++++ NNF Expression Tree Conversion ++++");
    etN = NNF(etI);
    printf("\nFormula in Negation Normal Form (NNF from Expression Tree):\n");
    ETP(etN);
    printf("\n");
    
    etDup = duplicate(etN); // keeping a duplicate copy for DNF conversion
    
    printf("\n++++ CNF Expression Tree Conversion ++++");
    etC = CNF(etN);
    printf("\nFormula in Conjunctive Normal Form (CNF from Expression Tree):\n");
    ETP(etC);
    printf("\n");
    

    printf("\n++++ DNF Expression Tree Conversion ++++");
    etD = DNF(etDup);
    printf("\nFormula in Disjunctive Normal Form (DNF from Expression Tree):\n");
    ETP(etD);
    printf("\n");


    printf("\n++++ Exhaustive Search from Expression Tree for Validity / Satisfiability Checking ++++\n");
    CHECK (et);
    
    /**********************************************************/   
    
    return 0;
}