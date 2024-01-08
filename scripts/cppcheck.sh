OUTPUT=$(cppcheck --enable=all --suppressions-list=suppressions.txt -iKRONOS/external/ -IKRONOS/ src/ 2>&1)

echo "$OUTPUT"

TOTAL_ERRORS=0

TOTAL_ERRORS=$(echo "$OUTPUT" | grep -o ": warning:" | wc -l)

echo "There were $TOTAL_ERRORS errors"

if (( TOTAL_ERRORS > 0 )); then
  exit 1
else
  exit 0
fi
