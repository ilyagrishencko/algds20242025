#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#define BUFFER_SIZE 256

typedef struct node {
    char* data;
    struct node* prev;
    struct node* next;
} Node;

void append(Node** head, const char* str) {
    if (*head == NULL) {
        Node* new_node = malloc(sizeof(Node));
        if (!new_node) {
            perror("Memory allocation error for the node");
            exit(EXIT_FAILURE);
        }

        new_node->data = _strdup(str);
        if (!new_node->data) {
            free(new_node);
            perror("Line copying error");
            exit(EXIT_FAILURE);
        }

        new_node->prev = NULL;
        new_node->next = NULL;
        *head = new_node;
        return;
    }

    Node* current = *head;
    while (current->next != NULL) {
        current = current->next;
    }

    Node* new_node = malloc(sizeof(Node));
    if (!new_node) {
        perror("Memory allocation error for the node");
        exit(EXIT_FAILURE);
    }

    new_node->data = _strdup(str);
    if (!new_node->data) {
        free(new_node);
        perror("Line copying error");
        exit(EXIT_FAILURE);
    }

    new_node->prev = current;
    new_node->next = NULL;
    current->next = new_node;
}

void swapNode(Node* first, Node* second) {
    char* FirstData = first->data;
    first->data = second->data;
    second->data = FirstData;
}

bool bubbleSort(Node** head, bool b) {
    if (b) {
        bool swapped = true;
        while (swapped) {
            swapped = false;
            Node* current = *head;
            while (current->next != NULL) {
                if (strcmp(current->data, current->next->data) > 0) {
                    swapNode(current, current->next);
                    swapped = true;
                }
                current = current->next;
            }
        }
        return true;
    }
    else{
        return false;
    }
}

int main() {
    Node* head = NULL;

    FILE* inputFile = fopen("TextFile1.txt", "r");
    if (!inputFile) {
        perror("Error opening the input file");
        return EXIT_FAILURE;
    }

    FILE* outputFile = fopen("TextFile2.txt", "w");
    if (!outputFile) {
        perror("Error opening the output file");
        fclose(inputFile);
        return EXIT_FAILURE;
    }

    char buffer[BUFFER_SIZE];
    bool b = false;

    while (fgets(buffer, sizeof(buffer), inputFile)) {
        b = true;
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }
        append(&head, buffer);
    }

    fclose(inputFile);

        fprintf(outputFile, "Unsorted list: ");
        for (Node* current = head; current != NULL; current = current->next) {
            fprintf(outputFile, "%s ", current->data);
        }
        fprintf(outputFile, "\n");

        bubbleSort(&head,b);

        fprintf(outputFile, "Sorted list: ");
        for (Node* current = head; current != NULL; current = current->next) {
            fprintf(outputFile, "%s ", current->data);
        }
    fclose(outputFile);

    while (head != NULL) {
        Node* temp = head;
        head = head->next;
        free(temp->data);
        free(temp);
    }

    //Unit tests
    test_append_empty_list(); //проверка добавления в пустой список
    test_append_nonempty_list(); //проверка добавления в непустой список
    test_bubbleSort(); //проверка сортировки для обычного (рандомного) списка
    test_bubbleSort_empty_list(); //empty_list пустой список
    test_bubbleSort_sorted_list(); //sorted_list список отсортированный
    test_bubbleSort_cosorted_list(); //cosorted_list список отсортированный в обратном порядке
    test_bubbleSort_constant_list(); //constant_list список из одинаковых элементов
    test_bubbleSort_one_element_list(); //one_element_list список из одного элемента
    test_swapNode_nonequals(); //проверка swapNode для разных значений
    test_swapNode_equals(); //проверка swapNode для одинаковых значений
    return 0;
}
//Unit tests

int test_append_empty_list() {
    Node* head1 = NULL;
    append(&head1, "A");
    assert((strcmp(head1->data, "A") == 0) && (head1->prev == NULL) && (head1->next == NULL));
}
int test_append_nonempty_list() {
    Node* head2 = NULL;
    append(&head2, "A");
    append(&head2, "B");
    assert((strcmp(head2->next->data, "B") == 0) && (head2->next->prev == head2) && (head2->next->next == NULL));
}
int test_bubbleSort_empty_list() {
    assert(!bubbleSort(NULL, false));
}
int test_bubbleSort() {
    Node a = { "B", NULL,NULL };
    Node b = { "A", &a,NULL };
    Node c = { "C", &b,NULL };
    a.next = &b;
    b.next = &c;
    Node* A = &a;
    bubbleSort(&A, true);
    assert((strcmp(A->data, "A") == 0) && (strcmp(A->next->data, "B") == 0) && (strcmp(A->next->next->data, "C") == 0));
}
int test_bubbleSort_sorted_list() {
    Node a = { "A", NULL,NULL };
    Node b = { "B", &a,NULL };
    a.next = &b;
    Node* A = &a;
    bubbleSort(&A, true);
    assert((strcmp(A->data, "A") == 0) && (strcmp(A->next->data, "B") == 0));
}
int test_bubbleSort_cosorted_list() {
    Node a = { "B", NULL,NULL };
    Node b = { "A", &a,NULL };
    a.next = &b;
    Node* A = &a;
    bubbleSort(&A, true);
    assert((strcmp(A->data, "A") == 0) && (strcmp(A->next->data, "B") == 0));
}
int test_bubbleSort_constant_list() {
    Node a = { "A", NULL,NULL };
    Node b = { "A", &a,NULL };
    a.next = &b;
    Node* A = &a;
    bubbleSort(&A , true);
    assert((strcmp(A->data, "A") == 0) && (strcmp(A->next->data, "A") == 0));
}
int test_bubbleSort_one_element_list() {
    Node a = { "A", NULL,NULL };
    Node* A = &a;
    bubbleSort(&A, true);
    assert((strcmp(A->data, "A") == 0));
}
int test_swapNode_nonequals() {
    Node a = { "A", NULL,NULL };
    Node b = { "B", &a,NULL };
    a.next = &b;
    swapNode(&a, a.next);
    assert((strcmp(a.data, "B") == 0) && (strcmp(a.next->data, "A") == 0));
}
int test_swapNode_equals() {
    Node a = { "A", NULL,NULL };
    Node b = { "A", &a,NULL };
    a.next = &b;
    swapNode(&a, a.next);
    assert((strcmp(a.data, "A") == 0) && (strcmp(a.next->data, "A") == 0));
}
