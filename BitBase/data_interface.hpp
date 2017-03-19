//
//  data_interface.hpp
//  BitBase
//
//  Created by lorabit on 31/01/2017.
//  Copyright © 2017 lorabit. All rights reserved.
//

#ifndef data_interface_hpp
#define data_interface_hpp

#include <string>

#include <stdio.h>
#include "versioned_value.hpp"

using namespace std;

int init_database();

void data_set(string key, int value);
versioned_value data_get(string key);
bool data_cas(string key, versioned_value value);

void inspect();

string process_request(string request);

#endif /* data_interface_hpp */
