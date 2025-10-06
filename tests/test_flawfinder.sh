#!/bin/bash
echo "Running flawfinder..."
flawfinder --minlevel=4 --error-level=4 ../src/ || exit 1
exit 0