tudentName) == 1);

  /* Check that the name starts with a letter. This only checks the
     first character (not every character in the name), since
     checking the whole string would require a loop, which is out
     of scope for this sprint. */
  if (nameIsValid) {
    nameIsValid = (studentName[0] >= 'A' && 