# utf8char
A simple utf8 char stream reader to test my UTF-8 understanding
```
utf8char < beer+pizza
🍺&🍕

utf8char , < beer+pizza   # add prefix string ","
,🍺,&,🍕,


utfchar [ ] < beer+pizza   # add prefix string "[" and postfix string "]"
[🍺][&][🍕][
]
```
