#include "all.h"

char *searchByKeyAndRelease(Table *tbl, char *keyToFind, int relToFind){
	if (tbl == NULL || tbl->ks == NULL) return NULL;
	for (int i=0; i<tbl->csize; i++){
		if (strcmp(tbl->ks[i].key, keyToFind) == 0){
			Node *elem = tbl->ks[i].node;
			while (elem != NULL){
				if (elem->release == relToFind){
					return strdup(elem->info);
				}
				elem = elem->next;
			}
		}
	}
	return NULL;
}





Table *searchAllByKey(Table* tbl, char* keyToFind) {
    if (tbl == NULL || keyToFind == NULL) return NULL;

    KeySpace* foundKS = NULL;

    for (int i = 0; i < tbl->csize; i++) {
        if (tbl->ks[i].key && strcmp(tbl->ks[i].key, keyToFind) == 0) {
            foundKS = &(tbl->ks[i]);
            break;
        }
    }

    if (!foundKS) return NULL;

    Table* resultTable = malloc(sizeof(Table));
    if (!resultTable) return NULL;

    resultTable->msize = 1;
    resultTable->csize = 1;
    resultTable->ks = malloc(sizeof(KeySpace));
    if (!resultTable->ks) {
        free(resultTable);
        return NULL;
    }

    resultTable->ks[0].key = strdup(keyToFind);
    if (!resultTable->ks[0].key) {
        free(resultTable->ks);
        free(resultTable);
        return NULL;
    }

    Node* srcNode = foundKS->node;
    Node** dstNode = &(resultTable->ks[0].node);
    *dstNode = NULL;
    int error = 0;

    while (srcNode && !error) {
        *dstNode = malloc(sizeof(Node));
        if (!*dstNode) {
            error = 1;
            break;
        }

        (*dstNode)->release = srcNode->release;
        (*dstNode)->info = strdup(srcNode->info);
        if (!(*dstNode)->info) {
            free(*dstNode);
            error = 1;
            break;
        }

        dstNode = &((*dstNode)->next);
        srcNode = srcNode->next;
    }

    if (error) {
        Node* curr = resultTable->ks[0].node;
        while (curr) {
            Node* next = curr->next;
            free(curr->info);
            free(curr);
            curr = next;
        }
        free(resultTable->ks[0].key);
        free(resultTable->ks);
        free(resultTable);
        return NULL;
    }

    *dstNode = NULL;
    return resultTable;
}
