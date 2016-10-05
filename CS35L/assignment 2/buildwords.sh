#!/bin/sh

# extract lines with <td></td> to find words
grep "<td>.*</td>" |

# replace Hawaiian ` letter to ASCII '
tr '`' "'" |

# replace upper case chars to all lower case chars
tr '[:upper:]' '[:lower:]' |

# remove HTML tags
sed 's/<[^>]*>//g' |

# remove backslashes(\)
sed 's/\///g' |

# remove <td>, by this, it only leaves words
sed 's/<td>//g' |

# remove <u>
sed 's/<u>//g' |

# remove empty new lines
# by this, we can only focus on the even number of lines
# for Hawaiian words
sed '/^\s*$/d' |

# remove odd number of lines
# by this, we can remove English words and only leave Hawaiian words
sed -n 'g;n;p' |

# remove the first 4 spaces
sed 's/^\s*//g' |

# since the assignment spec says we need to treat words with commas 
# or spaces as multiple words, we will first remove commas and replace
# all spaces into new lines

# remove commas
sed 's/,//g' |

# replace space into new line
sed 's/ /\n/g' |

# remove empty new line
# there was an extra empty line, so I removed it
sed '/^\s*$/d' |

# reject words that don't contain hawaiian characters
sed "/[^p^k^'^m^n^w^l^h^a^e^i^o^u]/d" |

# sort words and only display them once (remove duplicates)
sort -u
