#include <QtCore>

#include "tidy_wrapper.h"

/*
  if parentNode == NULL, search start from body node
  if class_name == NULL , omit the label's attr
  if class_name != NULL, then class_name's length must > 0
  label is TidyTag_XXX
 */
TidyNode searchNode(TidyDoc tdoc, TidyNode parentNode, const char * class_name, int label)
{
    TidyNode tnode = NULL;
    TidyNode currNode = NULL;
    TidyNode childNode = NULL;
    TidyAttr attr = NULL;
    ctmbstr value = NULL;

    assert(tdoc != NULL);
    if(class_name != NULL) {
        assert(strlen(class_name) > 0);
    }

    if(parentNode == NULL) {
        currNode = tidyGetBody(tdoc);
        assert(currNode != NULL);
    }else{
        currNode = parentNode;
    }

    for(childNode = tidyGetChild(currNode); childNode ; childNode = tidyGetNext(childNode)) {
        if(tidyNodeGetId(childNode) == label) {
            if(class_name == NULL) {
                return childNode;
            }
            for(attr = tidyAttrFirst(childNode); attr; attr = tidyAttrNext(attr)) {
                if(tidyAttrGetId(attr) == TidyAttr_CLASS
                   ||
                   tidyAttrGetId(attr) == TidyAttr_ID) {
                    value = tidyAttrValue(attr);
                    if(strcasecmp(value, class_name) == 0) {
                        return childNode;
                    }
                    value = NULL;
                }
            }
        }
    }
    for(childNode = tidyGetChild(currNode); childNode; childNode = tidyGetNext(childNode)) {
        tnode = searchNode(tdoc, childNode, class_name, label);
        if(tnode != NULL) {
            break;
        }
    }    

    return tnode;
}

TidyNode searchParentNode(TidyDoc tdoc, TidyNode childNode, const char * class_name, int label)
{
    TidyNode tnode = NULL;    
    TidyNode parentNode = NULL;
    TidyAttr attr = NULL;
    ctmbstr value = NULL;

    assert(childNode != NULL);
    assert(class_name != NULL);
    assert(strlen(class_name) > 0);
    
    for(parentNode = tidyGetParent(childNode); parentNode; parentNode = tidyGetParent(parentNode)) {
        if(tidyNodeGetId(parentNode) == label) {
            for(attr = tidyAttrFirst(parentNode); attr; attr = tidyAttrNext(attr)) {
                if(tidyAttrGetId(attr) == TidyAttr_CLASS
                   ||
                   tidyAttrGetId(attr) == TidyAttr_ID) {
                    value = tidyAttrValue(attr);
                    if(strcasecmp(value, class_name) == 0) {
                        return parentNode;
                    }
                    value = NULL;
                }
            }
        }        
    }

    return tnode;
}

TidyNode searchNext(TidyDoc tdoc, TidyNode node)
{
    TidyNode tnode = NULL;
    TidyNode nnode = NULL;
    TidyAttr attr = NULL;
    ctmbstr  class_name = NULL;
    ctmbstr  curr_class_name = NULL;
    
    assert(tdoc != NULL);
    assert(node != NULL);

    for(attr = tidyAttrFirst(node); attr ; attr= tidyAttrNext(attr)) {
        if(tidyAttrGetId(attr) == TidyAttr_CLASS) {
            class_name = tidyAttrValue(attr);
            break;
        }
    }
    //Q_ASSERT(class_name != NULL);
    assert(class_name != NULL);

    for(tnode = tidyGetNext(node); tnode; tnode = tidyGetNext(tnode)) {
        for(attr = tidyAttrFirst(tnode); attr ; attr= tidyAttrNext(attr)) {
            if(tidyAttrGetId(attr) == TidyAttr_CLASS) {
                curr_class_name = tidyAttrValue(attr);
                if(strcasecmp(curr_class_name, class_name) ==0 ) {
                    return tnode;
                }
            }
        }        
    }

    return NULL;
}


void get_node_text(TidyDoc tdoc, TidyNode node, TidyBuffer *buf)
{
    TidyBuffer text = {0};    
    tidyBufInit(&text);
    TidyNode child;    

    switch(tidyNodeGetType(node)) {
    case TidyNode_Text:
        //q_debug()<<"text found";
        tidyNodeGetText(tdoc, node, &text);
        if(text.bp[text.size-1] == '\n') {
            text.size -= 1;
        }
        tidyBufAppend(buf, text.bp, text.size);
        break;
    default:
        switch(tidyNodeGetId(node)) {
        case TidyTag_A:
            //q_debug()<<"a node found";
            get_node_text(tdoc, tidyGetChild(node), buf);        
            break;
        case TidyTag_P:
        case TidyTag_SPAN:
            get_node_text(tdoc, tidyGetChild(node), buf);
            break;
        case TidyTag_Q:
            get_node_text(tdoc, tidyGetChild(node), buf);
            break;
        case TidyTag_PLAINTEXT:
            //q_debug()<<"plain text found";
            tidyNodeGetText(tdoc, node, &text);
            tidyBufAppend(buf, text.bp, text.size);
            break;
        case TidyTag_H4:
        case TidyTag_H6:
        case TidyTag_DIV:
            for(child = tidyGetChild(node) ; child ; child = tidyGetNext(child)) {
                get_node_text(tdoc, child, buf);
            }
            break;
        case TidyTag_IMG:
            //q_debug()<<"it is img";
            break;
        default:
            //q_debug()<<"what is it?";
            printf("what is it?\n");
            break;
        }
        break;
    };
    tidyBufFree(&text);
}

void get_node_text2(TidyDoc tdoc, TidyNode node, TidyBuffer *buf, TidyBuffer *dbuf)
{
    TidyBuffer text = {0};    
    TidyNode child;
    TidyAttr  attr;

    tidyBufInit(&text);    
    switch(tidyNodeGetType(node)) {
    case TidyNode_Text:
        //q_debug()<<"text found";
        tidyNodeGetText(tdoc, node, &text);
        if(text.bp[text.size-1] == '\n') {
            text.size -= 1;
        }
        tidyBufAppend(buf, text.bp, text.size);
        break;
    default:
        switch(tidyNodeGetId(node)) {
        case TidyTag_SPAN:
            //q_debug()<<"a node found";
            attr = tidyAttrFirst(node);
            if(attr && tidyAttrGetId(attr) == TidyAttr_CLASS) {
                if(strcasecmp(tidyAttrValue(attr), "date") == 0) {
                    //q_debug()<<" got a date";
                    if(dbuf->size != 0 ) {
                        //Q_ASSERT(dbuf->size == 0);
                        tidyBufClear(dbuf);
                    }
                    tidyNodeGetText(tdoc, tidyGetChild(node), dbuf);
                    break;
                }
            }
        case TidyTag_H2:
        case TidyTag_H3:
        case TidyTag_A:
            //q_debug()<<"a node found";
            //get_node_text2(tdoc, tidyGetChild(node), buf);        
            for(child = tidyGetChild(node) ; child ; child = tidyGetNext(child)) {
                get_node_text2(tdoc, child, buf, dbuf);
            }
            break;
        case TidyTag_P:
            get_node_text2(tdoc, tidyGetChild(node), buf, dbuf);
            break;
        case TidyTag_Q:
            get_node_text2(tdoc, tidyGetChild(node), buf, dbuf);
            break;
        case TidyTag_PLAINTEXT:
            //q_debug()<<"plain text found";
            tidyNodeGetText(tdoc, node, &text);
            tidyBufAppend(buf, text.bp, text.size);
            break;
        case TidyTag_H6:
        case TidyTag_LI:
        case TidyTag_DIV:
            for(child = tidyGetChild(node) ; child ; child = tidyGetNext(child)) {
                get_node_text2(tdoc, child, buf, dbuf);
            }
            break;
        default:
            break;
        }
        break;
    };
    tidyBufFree(&text);
}


void get_node_text3(TidyDoc tdoc, TidyNode node, TidyBuffer *buf, const char *date_class, TidyBuffer *dbuf)
{
    TidyBuffer text = {0};    
    TidyNode child;
    TidyAttr  attr;
    ctmbstr ptr = NULL;

    tidyBufInit(&text);    
    switch(tidyNodeGetType(node)) {
    case TidyNode_Text:
        //q_debug()<<"text found";
        tidyNodeGetText(tdoc, node, &text);
        if(text.bp[text.size-1] == '\n') {
            text.size -= 1;
        }
        tidyBufAppend(buf, text.bp, text.size);
        break;
    default:
        switch(tidyNodeGetId(node)) {
        case TidyTag_A:
            //if javascript , omit link, only text
            //attr = tidyAttrGetById(node, TidyAttr_HREF);
            attr = tidyAttrGetHREF(node);
            if(attr != NULL) {
                ptr = tidyAttrValue(attr);
                if(ptr != NULL) {
                    if(strlen(ptr) >= 10 && qstrnicmp(ptr, "javascript", 10) == 0) {
                        get_node_text3(tdoc, tidyGetChild(node), buf, date_class, dbuf);
                        break;
                    }
                    if(ptr[0] == '#') { //  anchor  tag
                        get_node_text3(tdoc, tidyGetChild(node), buf, date_class, dbuf);
                        break;
                    }
                }
            }
            attr = NULL; ptr = NULL;
            tidyBufClear(&text);
            tidyNodeGetText(tdoc, node, &text);
            tidyBufAppend(buf, text.bp, text.size);            
            tidyBufClear(&text);
            break;
        case TidyTag_SPAN:
            //q_debug()<<"a node found";
            attr = tidyAttrFirst(node);
            if(attr && tidyAttrGetId(attr) == TidyAttr_CLASS) {
                if(strcasecmp(tidyAttrValue(attr), date_class) == 0) {
                    //q_debug()<<" got a date";
                    if(dbuf->size != 0 ) {
                        //Q_ASSERT(dbuf->size == 0);
                        tidyBufClear(dbuf);
                    }
                    tidyNodeGetText(tdoc, tidyGetChild(node), dbuf);
                    break;
                }
            }
            goto get_all_child_text;
            break;
        case TidyTag_PLAINTEXT:
            //q_debug()<<"plain text found";
            tidyNodeGetText(tdoc, node, &text);
            tidyBufAppend(buf, text.bp, text.size);
            break;
        case TidyTag_P:
            if(buf->size > 0) {
                tidyBufAppend(buf, (void*)"<BR>", 4);
            }
        case TidyTag_H2:
        case TidyTag_H3:
        case TidyTag_H6:
        //case TidyTag_A:
        case TidyTag_Q:
        case TidyTag_UL:
        case TidyTag_LI:
        case TidyTag_DIV:
        get_all_child_text:
            for(child = tidyGetChild(node) ; child ; child = tidyGetNext(child)) {
                get_node_text3(tdoc, child, buf, date_class, dbuf);
            }
            break;
        default:
            break;
        }
        break;
    };
    tidyBufFree(&text);
    
}
