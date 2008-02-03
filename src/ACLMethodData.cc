/*
 * $Id: ACLMethodData.cc,v 1.11 2008/02/03 10:00:29 amosjeffries Exp $
 *
 * DEBUG: section 28    Access Control
 * AUTHOR: Duane Wessels
 *
 * SQUID Web Proxy Cache          http://www.squid-cache.org/
 * ----------------------------------------------------------
 *
 *  Squid is the result of efforts by numerous individuals from
 *  the Internet community; see the CONTRIBUTORS file for full
 *  details.   Many organizations have provided support for Squid's
 *  development; see the SPONSORS file for full details.  Squid is
 *  Copyrighted (C) 2001 by the Regents of the University of
 *  California; see the COPYRIGHT file for full details.  Squid
 *  incorporates software developed and/or copyrighted by other
 *  sources; see the CREDITS file for full details.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *  
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111, USA.
 *
 *
 * Copyright (c) 2003, Robert Collins <robertc@squid-cache.org>
 */

#include "squid.h"
#include "ACLMethodData.h"
#include "ACLChecklist.h"
#include "HttpRequestMethod.h"
#include "wordlist.h"

ACLMethodData::ACLMethodData() : values (NULL)
{}

ACLMethodData::ACLMethodData(ACLMethodData const &old) : values (NULL)
{
    assert (!old.values);
}

ACLMethodData::~ACLMethodData()
{
    if (values)
        delete values;
}

/// todo make this a pass-by-reference now that HTTPRequestMethods a full class?
bool
ACLMethodData::match(HttpRequestMethod toFind)
{
    return values->findAndTune(toFind);
}

/* explicit instantiation required for some systems */

template cbdata_type List<HttpRequestMethod>
::CBDATA_List;

wordlist *
ACLMethodData::dump()
{
    wordlist *W = NULL;
    List<HttpRequestMethod> *data = values;

    while (data != NULL) {
        wordlistAdd(&W, RequestMethodStr(data->element));
        data = data->next;
    }

    return W;
}

void
ACLMethodData::parse()
{
    List<HttpRequestMethod> **Tail;
    char *t = NULL;

    for (Tail = &values; *Tail; Tail = &((*Tail)->next))

        ;
    while ((t = strtokFile())) {
        List<HttpRequestMethod> *q = new List<HttpRequestMethod> (HttpRequestMethod(t, NULL));
        *(Tail) = q;
        Tail = &q->next;
    }
}

bool
ACLMethodData::empty() const
{
    return values == NULL;
}

ACLData<HttpRequestMethod> *
ACLMethodData::clone() const
{
    assert (!values);
    return new ACLMethodData(*this);
}
