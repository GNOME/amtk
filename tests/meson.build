interactive_tests = [
  # executable name, sources
  ['test-headerbar', 'test-headerbar.c'],
  ['test-traditional-ui', 'test-traditional-ui.c'],
]

foreach test : interactive_tests
  executable(
    test[0], test[1],
    dependencies: AMTK_DEP,
  )
endforeach
