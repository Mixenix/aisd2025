#include "all.h"


int removeByKey(Table *tbl, char *keyToDel){
	if (tbl == NULL || tbl->ks == NULL){return BAD;};
	for (int i=0; i<tbl->csize; i++){
		if (strcmp(tbl->ks[i].key, keyToDel) == 0){
			Node *elem = tbl->ks[i].node;
			while (elem != NULL){
				Node *tmp = elem->next;
				free(elem->info);
				free(elem);
				tbl->csize--;
				elem = tmp;
			}
			free(tbl->ks[i].key);
			return GOOD;
		}
	}
	return BAD;
}

int removeByKeyAndRelease(Table *tbl, char *keyToDel, int RelToDel){
	if (tbl == NULL || tbl->ks == NULL){return BAD;};
	for (int i = 0; i < tbl->csize; i++) {
        if (tbl->ks[i].key && strcmp(tbl->ks[i].key, keyToDel) == 0) {
            Node** ptr = &(tbl->ks[i].node);
            while (*ptr) {
                if ((*ptr)->release == RelToDel) {
                    Node* toDelete = *ptr;
                    *ptr = toDelete->next;
                    free(toDelete->info);
                    free(toDelete);
                    
                    if (tbl->ks[i].node == NULL) {
                        free(tbl->ks[i].key);
                        tbl->ks[i].key = NULL;
                    }
                    return GOOD;
                }
                ptr = &((*ptr)->next);
            }
            break;
        }
    }
    return BAD;
}
