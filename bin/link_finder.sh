name=$1
grep -Eoi '<a [^>]+>' $name |
grep -Eo 'href="[^\"]+"' | 
grep -Eo '(http|https)://[^/"]+'