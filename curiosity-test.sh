passed=0
failed=0

for t in test_*.txt; do
  [ -e "$t" ] || { echo "No tests found (pattern $t)"; break; }
  echo ""
  echo "=== $t ==="
  if ./curiosity-test "$t"; then
    ((passed++))
  else
    ((failed++))
  fi
  echo ""
done

echo "Summary: $passed passed, $failed failed"
if (( failed > 0 )); then
  exit 1
else
  exit 0
fi