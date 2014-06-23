/*** DlList routines ***/

/* Makes a new, empty list */
DlList *createDlList() {
    DlList *dlList = malloc(sizeof(DlList));
    if(dlList) {
	dlList->head = &(dlList->data);
	dlList->tail = dlList->head;
	dlList->head->next = NULL;
	dlList->head->prev = NULL;
	dlList->count = 0;
    }
    return dlList;
}

/* Put element at the end of the list (Doesn't create element) */
void appendDlElement(DlList *l, DlElement *p)
{
    p->prev = l->tail;
    p->next = NULL;
    l->tail->next = p;
    l->tail = p;
    l->count++;
}

/* Put element at the head of the list */
void insertDlElement(DlList *l, DlElement *p)
{
    p->prev = l->head;
    p->next = l->head->next;
    if(l->tail == l->head) {
	l->tail = p;
    } else {
	l->head->next->prev = p;
    }
    l->head->next = p;
    l->count++;
}

/* Adjusts pointers to include (Doesn't create element) */
void insertAfter(DlList *l, DlElement *p1, DlElement *p2)
{
    p2->prev = p1;
    p2->next = p1->next;
    if(l->tail == p1) {
	l->tail = p2;
    } else {
	p1->next->prev = p2;
    }
    p1->next = p2;
    l->count++;
}

/* Moves elements from src to after an element in dest */
void insertDlListAfter(DlList *pDest, DlElement *p, DlList *pSrc)
{
    if(IsEmpty(pSrc)) return;
    FirstDlElement(pSrc)->prev = p;
    LastDlElement(pSrc)->next = p->next;
    if(pDest->tail == p) {
	pDest->tail = LastDlElement(pSrc);
    } else {
	p->next->prev = LastDlElement(pSrc);
    }
    p->next = FirstDlElement(pSrc);
    pDest->count += pSrc->count;
    emptyDlList(pSrc);
}

/* Moves elements from src to the end of dest */
void appendDlList(DlList *pDest, DlList *pSrc)
{
    DlElement *p;
    if(pSrc->count <= 0) return;
    pDest->count += pSrc->count;
    pSrc->count = 0;
    p = pSrc->head->next;
    pSrc->head->next = NULL;
    p->prev = pDest->tail;
    pDest->tail->next = p;
    pDest->tail = pSrc->tail;
    pSrc->tail = pSrc->head;
}

/* Adjusts pointers to be empty list (Doesn't free elements) */
void emptyDlList(DlList *l)
{
    l->head->next = 0;
    l->tail = l->head;
    l->count = 0;
}

/* Adjusts pointers to eliminate element (Doesn't free element) */
void removeDlElement(DlList *l, DlElement *p)
{
    l->count--;
    p->prev->next = p->next;
    if(l->tail == p) {
	l->tail = p->prev;
    } else {
	p->next->prev = p->prev;
    }
    p->next = p->prev = 0;
}

/* Prints the elements in a list */
void dumpDlElementList(DlList *l)
{
    DlElement *p = 0;
    int i = 0;
    print("Number of Elements = %d\n",l->count);
    p = FirstDlElement(l);
    while(p) {
	if(p->type == DL_Element) {
	    print("%03d (%s) [%x] x=%d y=%d width=%u height=%u\n",i++,
	      elementType(p->structure.element->type),
	      p->structure.element->structure.composite,
	      p->structure.element->structure.composite->object.x,
	      p->structure.element->structure.composite->object.y,
	      (int)p->structure.element->structure.composite->object.width,
	      (int)p->structure.element->structure.composite->object.height);
	} else {
	    print("%03d %s [%x] x=%d y=%d width=%u height=%u\n",i++,
	      elementType(p->type),
	      p->structure.composite,
	      p->structure.composite->object.x,
	      p->structure.composite->object.y,
	      (int)p->structure.composite->object.width,
	      (int)p->structure.composite->object.height);
	}
	p = p->next;
    }
    return;
}

