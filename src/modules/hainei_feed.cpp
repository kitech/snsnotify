#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <memory.h>
#include <tidy.h>
#include "tidy_wrapper.h"
#include "feed_record.h"

#include "sns_feed.h"

int get_version()
{
    TidyDoc doc;
    return 123;
}

TidyNode first_feed(TidyDoc doc)
{
    TidyNode node = NULL;

    

    return node;
}

TidyNode next_feed(TidyDoc doc, TidyNode prev)
{
    TidyNode node = NULL;

    node = tidyGetNext(prev);
    
    return node;
}

const char * parse_feed(TidyDoc doc, TidyNode node)
{
    char * feed_text = NULL;

    return feed_text;
}


/*
  TidyNode node;
  for(node = first_feed(doc); node; node = next_feed(doc, node))
  {
      const char * feed_text = parse_feed(doc, node);
  }  
 */

//////////////
site_const_t * get_site_const(int step)
{
    site_const_t * sc = site_const_new();
    assert(sc != NULL);

    sc->get_version = get_version;
    sc->scfree = site_const_free;

    sc->step = step;
    assert(step == 0);
    sc->next = 0;

    sc->site_name = "hainei";
    sc->site_icon = "hainei/favicon.ico";
    sc->home_page = "http://www.hainei.com/";
    sc->encoding = "UTF-8";
    sc->host = "www.hainei.com";
    sc->port = 80;
    sc->login_url = "/account?a=login&h=45eb3713";
    sc->feed_url = "/home";
    sc->cookies = "s=ege7rf5ev24bjck3b8gl4jh7t1";
    
    sc->data = "email=%1&password=%2&autologin=on";

    sc->letter_urls = (char**)malloc(10*sizeof(char*));
    memset(sc->letter_urls, 0, (10*sizeof(char*)));

    return sc;
}



