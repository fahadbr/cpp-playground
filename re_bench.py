#!/usr/bin/env python3

import re
import sys

# for benchmarking python regex
val = "2020-06-30"
for i in range(1000000):
    if not re.match("[0-9]{4}-[0-9]{2}-[0-9]{2}", val):
        sys.exit(1)
