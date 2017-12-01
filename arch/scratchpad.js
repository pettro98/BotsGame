qq = {noll:0, an:1, de:2, ti:3, go:4, su:5, by:6, ra:7, me:8, ni:9, ko:a, hu:b, vy:c, la:d, po:e, fy:f, ton :10};
str = "go by go vy go an go ra ra hu by vy by ni by hu by su su fy by an su fy ra ti by ni by la ra noll by vy by su su fy by me by su ra me ra la";

str = str.split(' ');

for(i in str){
    str[i] = qq[str[i]];
}

str = str.join(' ');

/*
Exception: ReferenceError: a is not defined
@Scratchpad/2:1:69
*/