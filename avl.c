#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// --- Estrutura e Funções da Árvore AVL ---

typedef struct Node {
    int key;
    struct Node *left;
    struct Node *right;
    int height;
} Node;

int height(Node *N) {
    if (N == NULL)
        return 0;
    return N->height;
}

int max(int a, int b) {
    return (a > b) ? a : b;
}

Node* newNode(int key) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->key   = key;
    node->left  = NULL;
    node->right = NULL;
    node->height = 1;
    return (node);
}

int getBalance(Node *N) {
    if (N == NULL)
        return 0;
    return height(N->left) - height(N->right);
}

Node *rightRotate(Node *y) {
    Node *x = y->left;
    Node *T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    return x;
}

Node *leftRotate(Node *x) {
    Node *y = x->right;
    Node *T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    return y;
}

Node* insert(Node* node, int key) {
    if (node == NULL)
        return(newNode(key));

    if (key < node->key)
        node->left  = insert(node->left, key);
    else if (key > node->key)
        node->right = insert(node->right, key);
    else
        return node;

    node->height = 1 + max(height(node->left),
                           height(node->right));

    int balance = getBalance(node);

    if (balance > 1 && key < node->left->key)
        return rightRotate(node);

    if (balance < -1 && key > node->right->key)
        return leftRotate(node);

    if (balance > 1 && key > node->left->key) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    if (balance < -1 && key < node->right->key) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

Node *minValueNode(Node* node) {
    Node* current = node;

    while (current->left != NULL)
        current = current->left;

    return current;
}

Node* deleteNode(Node* root, int key) {
    if (root == NULL)
        return root;

    if (key < root->key)
        root->left = deleteNode(root->left, key);

    else if (key > root->key)
        root->right = deleteNode(root->right, key);

    else {
        if ((root->left == NULL) || (root->right == NULL)) {
            Node *temp = root->left ? root->left : root->right;

            if (temp == NULL) {
                temp = root;
                root = NULL;
            }
            else
                *root = *temp;
            free(temp);
        }
        else {
            Node* temp = minValueNode(root->right);

            root->key = temp->key;

            root->right = deleteNode(root->right, temp->key);
        }
    }

    if (root == NULL)
        return root;

    root->height = 1 + max(height(root->left),
                           height(root->right));

    int balance = getBalance(root);

    if (balance > 1 && getBalance(root->left) >= 0)
        return rightRotate(root);

    if (balance > 1 && getBalance(root->left) < 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    if (balance < -1 && getBalance(root->right) <= 0)
        return leftRotate(root);

    if (balance < -1 && getBalance(root->right) > 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}

Node* search(Node* root, int key) {
    if (root == NULL || root->key == key)
       return root;

    if (root->key < key)
       return search(root->right, key);

    return search(root->left, key);
}

void freeTree(Node *root) {
    if (root != NULL) {
        freeTree(root->left);
        freeTree(root->right);
        free(root);
    }
}

// --- Função Principal para Análise Empírica ---

int main() {
    int sizes[] = {1, 5, 10, 25, 50, 100, 250, 1000, 2500, 5000, 7500, 10000};
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);
    int max_key = 1000000; // Aumentei o intervalo para garantir mais exclusividade de chaves
    
    srand(time(NULL));

    printf("N\tTempo Insercao (ms)\tTempo Pesquisa (ms)\tTempo Exclusao (ms)\n");
    printf("------------------------------------------------------------------\n");

    for (int i = 0; i < num_sizes; i++) {
        int N = sizes[i];
        Node *root = NULL;
        int *keys = (int *)malloc(N * sizeof(int));
        
        clock_t start, end;
        double cpu_time_used;

        // INSERÇÃO
        start = clock();
        for (int j = 0; j < N; j++) {
            int key = (rand() % max_key) + 1;
            keys[j] = key;
            root = insert(root, key);
        }
        end = clock();
        cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC * 1000;
        double insert_time = cpu_time_used;
        
        // PESQUISA
        start = clock();
        for (int j = 0; j < N; j++) {
            search(root, keys[j]); 
        }
        end = clock();
        cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC * 1000;
        double search_time = cpu_time_used;

        // EXCLUSÃO
        start = clock();
        for (int j = 0; j < N; j++) {
            root = deleteNode(root, keys[j]);
        }
        end = clock();
        cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC * 1000;
        double delete_time = cpu_time_used;

        printf("%d\t%.4f\t\t%.4f\t\t%.4f\n", N, insert_time, search_time, delete_time);
        
        free(keys);
        
        // Embora deleteNode deva deixar root NULL no final, esta função garante a limpeza.
        freeTree(root);
    }

    return 0;
}
