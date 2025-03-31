#include "all.h"


Table *readTableFromFile(char *filename){
    FILE *file = fopen(filename, "r");
    if (!file) return NULL;

    Table* table = malloc(sizeof(Table));
    if (!table) {
        fclose(file);
        return NULL;
    }

    table->msize = 10;
    table->csize = 0;
    table->ks = calloc(table->msize, sizeof(KeySpace));
    if (!table->ks) {
        free(table);
        fclose(file);
        return NULL;
    }

    char* line;
    KeySpace* currentKS = NULL;
    int lineNum = 0;
    int error = 0;

    while ((line = readtxtline(file)) != NULL) {
        lineNum++;
        size_t len = strlen(line);

        if (len == 0) {
            free(line);
            continue;
        }

        if (line[0] == '|') {
            if (table->csize >= table->msize) {
                table->msize *= 2;
                KeySpace* newKS = realloc(table->ks, table->msize * sizeof(KeySpace));
                if (!newKS) {
                    error = 1;
                    free(line);
                    break;
                }
                table->ks = newKS;
            }

            currentKS = &table->ks[table->csize++];
            currentKS->key = strdup(line + 1);
            currentKS->node = NULL;
            
            if (!currentKS->key) {
                error = 1;
                free(line);
                break;
            }
        }
        else if (line[0] == '/') {
            if (!currentKS) {
                error = 1;
                free(line);
                break;
            }

            char* secondSlash = strchr(line + 1, '/');
            if (!secondSlash) {
                error = 1;
                free(line);
                break;
            }

            *secondSlash = '\0';
            char* endptr;
            int release = (int)strtol(line + 1, &endptr, 10);
            if (*endptr != '\0' || release < 0) {
                error = 1;
                free(line);
                break;
            }

            char* info = secondSlash + 1;
            if (strlen(info) == 0) {
                error = 1;
                free(line);
                break;
            }

            Node* newNode = malloc(sizeof(Node));
            if (!newNode) {
                error = 1;
                free(line);
                break;
            }

            newNode->release = release;
            newNode->info = strdup(info);
            newNode->next = NULL;

            if (!newNode->info) {
                free(newNode);
                error = 1;
                free(line);
                break;
            }

            if (!currentKS->node) {
                currentKS->node = newNode;
            } else {
                Node* last = currentKS->node;
                while (last->next) last = last->next;
                last->next = newNode;
            }
        } else {
            error = 1;
            free(line);
            break;
        }

        free(line);
    }

    if (error || (lineNum > 0 && table->csize == 0)) {
        for (int i = 0; i < table->csize; i++) {
            free(table->ks[i].key);
            Node* node = table->ks[i].node;
            while (node) {
                Node* next = node->next;
                free(node->info);
                free(node);
                node = next;
            }
        }
        free(table->ks);
        free(table);
        fclose(file);
        return NULL;
    }

    fclose(file);
    return table;
}

char *readtxtline(FILE *f){
	int cap = 32,  next = 0, c;
    char *p = malloc(cap);
    if (p == NULL){
    	return NULL;
    }
    while( 1 ) { 
        if ( next == cap ) {
            p = realloc( p, cap *= 2 );
        }
        c = fgetc( f ); 
        if ( c == EOF || c == '\n' ) {
            p[next++] = 0;
            break;
        }
        p[next++] = c;
    }
    if ( c == EOF && next == 1 ) {
        free( p );
        p = NULL;
    }
    return p;
}
