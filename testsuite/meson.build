unit_tests = [
  'test-action-info-store',
  'test-action-map',
  'test-utils',
]

foreach test_name : unit_tests
  test_exe = executable(
    test_name,
    test_name + '.c',
    dependencies: AMTK_DEP,
  )
  test(test_name, test_exe)
endforeach
