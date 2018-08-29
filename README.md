# utf8char
A simple utf8 char stream reader to test my understanding UTF-8
```
utf8char < beer+pizza
🍺&🍕

utf8char , < beer+pizza   # add prefix string ","
,🍺,&,🍕,


utfchar [ ] < beer+pizza   # add prefix string "[" and postfix string "]"
[🍺][&][🍕][
]
```
