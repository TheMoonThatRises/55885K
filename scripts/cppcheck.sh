OUTPUT=$(cppcheck --enable=all --suppress=missingInclude --suppress=missingIncludeSystem --suppress=checkersReport -iKRONOS/external/ -IKRONOS/ src/ 2>&1)
ERROR_LIST=("warning" "note" "style" "information")

echo "$OUTPUT"

TOTAL_ERRORS=0

for ERROR in "${ERROR_LIST[@]}"; do
  COUNT=$(echo "$OUTPUT" | grep -o ": $ERROR:" | wc -l)
  TOTAL_ERRORS=$((TOTAL_ERRORS + COUNT))
done

echo "There were $TOTAL_ERRORS errors"

if (( TOTAL_ERRORS > 0 )); then
  exit 1
else
  exit 0
fi
