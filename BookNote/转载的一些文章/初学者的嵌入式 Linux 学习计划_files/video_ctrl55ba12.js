define("biz_wap/jsapi/leaveReport.js",["biz_wap/utils/ajax.js","biz_wap/jsapi/core.js","biz_common/utils/url/parse.js","biz_wap/utils/log.js"],function(e){
"use strict";
function t(e){
var t={};
return"undefined"!=typeof uin&&(t.uin=uin),"undefined"!=typeof key&&(t.key=key),
"undefined"!=typeof pass_ticket&&(t.pass_ticket=pass_ticket),"undefined"!=typeof wxtoken&&(t.wxtoken=wxtoken),
"undefined"!=typeof window.devicetype&&(t.devicetype=window.devicetype),"undefined"!=typeof window.clientversion&&(t.clientversion=window.clientversion),
"undefined"!=typeof appmsg_token?t.appmsg_token=appmsg_token:e.indexOf("advertisement_report")>-1&&((new Image).src=location.protocol+"//mp.weixin.qq.com/mp/jsmonitor?idkey=68064_13_1&r="+Math.random()),
t.x5=l?"1":"0",t.f="json",f.join(e,t);
}
function o(e){
return e&&"object"==typeof e;
}
function n(e,t){
if(o(e)&&o(t))for(var n in t)Object.prototype.hasOwnProperty.call(t,n)&&(e[n]=t[n]);
}
function r(e){
u("[leaveReport 1]"),console.log("[leaveReport 1]");
var r={};
for(var i in y){
r[i]||(r[i]={});
for(var a=0;a<y[i].length;a++){
var p=y[i][a];
"function"==typeof p?n(r[i],p(e)):o(p)&&n(r[i],p);
}
}
u("[leaveReport getDataFunc.length "+_.length+"]"),console.log("[leaveReport getDataFunc.length "+_.length+"]");
for(var a=0;a<_.length;a++){
var s=_[a](e);
o(s)&&g.push(s);
}
for(var a=0;a<g.length;a++)g[a].reportUrl&&(g[a].reportUrl=t(g[a].reportUrl));
return r.data={
requestList:g
},r;
}
function i(e){
"function"==typeof e?_.push(e):o(e)&&g.push(e);
}
function a(e,t){
y[e]||(y[e]=[]),y[e].push(t);
}
function p(e){
var t=r(!0);
c.invoke("handleMPPageAction",{
action:"reportByLeaveForMPGateway",
reportData:t
},function(o){
if(o&&o.err_msg&&-1!==o.err_msg.indexOf(":ok"))_=[],g=[],y={},"function"==typeof e&&e(o);else{
_=[],g=[];
var n=t.data.requestList.length;
t.data.requestList.forEach(function(t){
t.reportUrl&&s({
type:t.method||"GET",
url:t.reportUrl,
data:t.reportData,
async:!1,
success:function(t){
--n<0&&"function"==typeof e&&e({
err_msg:"handleMPPageAction:ok",
fallback:!0,
resp:t
});
},
error:function(t,o){
--n<0&&"function"==typeof e&&e({
err_msg:"handleMPPageAction:fail",
fallback:!0,
err:o
});
}
});
});
}
});
}
var s=e("biz_wap/utils/ajax.js"),c=e("biz_wap/jsapi/core.js"),f=e("biz_common/utils/url/parse.js"),u=e("biz_wap/utils/log.js"),l=-1!=navigator.userAgent.indexOf("TBS/"),d={},v=!1;
try{
d=top.window.document;
}catch(w){
v=!0;
}
if(!v&&top.window.__leaveReport)return top.window.__leaveReport;
if(window.__leaveReport)return window.__leaveReport;
var _=[],g=[],y={};
c.on("reportOnLeaveForMP",function(){
return r(!1);
});
var h={
reportNow:p,
addReport:i,
addSpecificReport:a
};
return window.__leaveReport=h,h;
});define("biz_wap/utils/hand_up_state.js",["biz_common/dom/event.js"],function(n){
"use strict";
function e(){
if("hidden"in document)return"hidden";
for(var n=["webkit","moz","ms","o"],e=0;e<n.length;e++)return n[e]+"Hidden"in document,
n[e]+"Hidden";
return null;
}
function i(){
var n=e();
return n?document[n]:!1;
}
function t(){
return r;
}
var d=n("biz_common/dom/event.js"),o=e(),r=0,u=0;
if(o){
var m=o.replace(/[H|h]idden/,"")+"visibilitychange";
d.on(document,m,function(){
i()?u=(new Date).getTime():r+=(new Date).getTime()-u;
},!1);
}
return{
getHandUpTime:t,
isHidden:i
};
});define("biz_common/utils/http.js",[],function(){
"use strict";
function t(){
var t=document.getElementsByTagName("html");
if(t&&1==!!t.length){
t=t[0].innerHTML;
var e=t.replace(/[\x00-\xff]/g,""),n=t.replace(/[^\x00-\xff]/g,"");
return 1*n.length+3*e.length+"<!DOCTYPE html><html></html>".length;
}
return 0;
}
return{
htmlSize:t()
};
});define("biz_common/utils/cookie.js",[],function(){
"use strict";
var e={
get:function(e){
if(""==e)return"";
var t=new RegExp(e+"=([^;]*)"),n=document.cookie.match(t);
return n&&n[1]||"";
},
set:function(e,t,n){
var o=new Date;
return o.setDate(o.getDate()+(n||1)),n=o.toGMTString(),document.cookie=e+"="+t+";expires="+n,
!0;
}
};
return e;
});define("appmsg/open_url_with_webview.js",["biz_wap/jsapi/core.js"],function(e){
"use strict";
var r=e("biz_wap/jsapi/core.js"),n=-1!=navigator.userAgent.indexOf("WindowsWechat"),i=function(e,i){
if(n)return location.href=e,!1;
i=i||{};
var o=i.sample||0;
o*=1e3;
var t=window.user_uin||0,s=0!==t&&Math.floor(t/100)%1e3<o;
return s?void r.invoke("openUrlWithExtraWebview",{
url:e,
openType:i.openType||1,
scene:i.scene||"",
bizUsername:i.user_name||""
},function(e){
e&&"openUrlWithExtraWebview:ok"===e.err_msg?i.resolve&&i.resolve():i.reject&&i.reject();
}):void(i.reject&&i.reject());
};
return i;
});var _extends=Object.assign||function(e){
for(var n=1;n<arguments.length;n++){
var i=arguments[n];
for(var t in i)Object.prototype.hasOwnProperty.call(i,t)&&(e[t]=i[t]);
}
return e;
};
define("appmsg/album_keep_read.js",["page/appmsg_new/mod/album_read.css","biz_common/dom/event.js","biz_common/dom/class.js","pages/mod/bottom_modal.js","biz_wap/utils/openUrl.js","pages/utils.js","biz_wap/utils/ajax.js","common/comm_report.js","common/utils.js","biz_common/dom/offset.js","biz_wap/utils/wapsdk.js"],function(e){
"use strict";
function n(e,n){
v.jsmonitor({
id:223326,
key:e,
value:n
});
}
function i(){
var e=arguments.length<=0||void 0===arguments[0]?{}:arguments[0];
b.report(21277,_extends({},x,e));
}
function t(){
function e(){
if(!t){
var e=w.getOffset(h).offsetTop,o=f.getScrollTop();
o+f.getInnerHeight()>=e+a&&e+a>=o&&(i({
ActionType:1
}),n(11),t=!0);
}
}
var t=void 0,a=h.offsetHeight/2;
f.bindDebounceScrollEvent(e),e();
}
function a(){
var e=window.appmsg_album_info;
if(e.link){
var n=e.link.replace("#wechat_redirect","&subscene=159&subscene="+window.source+"&scenenote="+encodeURIComponent(window.location.href)+"&nolastread=1#wechat_redirect");
_.openUrlWithExtraWebview(n.htmlDecode());
}
}
function o(e){
e&&c.on(e,"click","."+y.itemClassName,function(e){
var n=e.delegatedTarget;
if(!u.hasClass(n,"album_read_directory_current")){
var t=void 0;
t=n.dataset.url.includes("#")?n.dataset.url.replace("#",T+"&scene=190#"):n.dataset.url+(T+"&scene=190"),
i({
ActionType:5,
Url:t
}),p.jumpUrl(t,!0),u.addClass(n,"album_read_directory_disabled");
}
});
}
function s(){
var e=document.createElement("div"),n=document.createElement("span");
return n.innerHTML="#"+window.appmsg_album_info.title,e.appendChild(n),e.innerHTML;
}
function r(e){
g({
type:"GET",
url:"/mp/appmsgalbum?action=paging&__biz="+window.biz+"&album_id="+y.albumId+"&cur_msgid="+y.msgid+"&cur_itemidx="+y.idx+"&count="+y.pageCount,
timeout:5e3,
dataType:"json",
success:function(n){
n.base_resp&&0===n.base_resp.ret&&e(n.getalbum_resp);
},
error:function(){
console.log("[error]");
}
});
}
function l(){
function e(e){
var n=arguments.length<=1||void 0===arguments[1]?!1:arguments[1],i=arguments.length<=2||void 0===arguments[2]?!0:arguments[2],t=document.createDocumentFragment();
if(_)if(n){
h=1;
for(var a=0;a<e.length;a++)if(e[a].msgid.toString()===y.msgid.toString()&&e[a].itemidx.toString()===y.idx.toString()){
h=a;
break;
}
e.forEach(function(e,n){
e.index=p?c-(n-h):c+(n-h);
});
}else e.forEach(i?function(e,n){
e.index=p?v.index-n-1:v.index+n+1;
}:function(n,i){
n.index=p?w.index+(e.length-i):w.index-(e.length-i);
});
return e.forEach(function(e){
var n=document.createElement("div");
if(u.addClass(n,y.itemClassName),e.msgid.toString()===window.mid.toString()&&e.itemidx.toString()===window.idx.toString()?u.addClass(n,"album_read_directory_current"):1===e.user_read_status&&u.addClass(n,"album_read_directory_disabled"),
n.innerText=_?e.index+"."+e.title:e.title,n.setAttribute("data-url",e.url),e.is_paid){
var i="已付费";
n.innerHTML+='<span class="wx_icon_pay_tag wx_icon_pay_tag_paid">'+i+"</span>";
}else if(e.is_pay_subscribe){
var a="付费";
n.innerHTML+='<span class="wx_icon_pay_tag">'+a+"</span>";
}
t.appendChild(n);
}),t;
}
function t(e){
1*e.reverse_continue_flag||(b=!1),1*e.continue_flag||(f=!1);
}
var l=document.createElement("div");
l.style.position="relative";
var d=[],c=void 0,_=void 0,p=void 0,b=!0,f=!0,w=void 0,v=void 0,h=1,x={
extClass:"album_read_directory",
hasBtn:!0,
btnSlot:'<div id="js_topic_detail" class="weui-btn__word-wrp">\n                  <span class="weui-btn__word">详情</span>\n                  <i class="weui_right_arrow"></i>\n                </div>',
title:s(),
btnClickCb:function(){
i({
ActionType:6
}),n(15),a();
},
cb:function(){
r(function(n){
j.hideLoading(),p=n.base_info.is_reverse,c=n.base_info.cur_article_num,_=c?1:0,l.appendChild(e(n.article_list,!0)),
w=n.article_list[0],v=n.article_list[n.article_list.length-1],t(n),f||j.showEndLine();
var i=l.childNodes[h];
setTimeout(function(){
i.offsetTop+i.clientHeight>j.contentAreaWrp.clientHeight&&j.scrollTo(0,l.offsetHeight);
},200);
});
},
onShow:function(){
i({
ActionType:4
}),n(14);
},
onPullUpLoad:function(){
f&&(j.showPullUpLoading(),g({
type:"GET",
url:"/mp/appmsgalbum?action=paging&__biz="+window.biz+"&album_id="+y.albumId+"&begin_msgid="+v.msgid+"&begin_itemidx="+v.itemidx+"&count="+y.pageCount,
timeout:5e3,
dataType:"json",
success:function(n){
j.hidePullUpLoading();
var i=n.base_resp&&1*n.base_resp.ret;
if(0===i){
var a=n.getalbum_resp.article_list;
t(n.getalbum_resp),l.appendChild(e(a,!1,!0)),d=d.concat(a),j.finishPullUpLoad(),
v=d[d.length-1],1*n.getalbum_resp.continue_flag||j.showEndLine();
}
},
error:function(){}
}));
},
onPullDownLoad:function(){
b&&(j.showPullDownLoading(),g({
type:"GET",
url:"/mp/appmsgalbum?action=paging&__biz="+window.biz+"&album_id="+y.albumId+"&begin_msgid="+w.msgid+"&begin_itemidx="+w.itemidx+"&count="+y.pageCount+"&isbackward=1",
timeout:5e3,
dataType:"json",
success:function(n){
j.hidePullDownLoading();
var i=n.base_resp&&1*n.base_resp.ret;
if(0===i){
var a=n.getalbum_resp.article_list,o=n.getalbum_resp.continue_flag;
n.getalbum_resp.continue_flag=n.getalbum_resp.reverse_continue_flag,n.getalbum_resp.reverse_continue_flag=o,
t(n.getalbum_resp);
var s=e(a,!1,!1),r=l.firstChild;
l.insertBefore(s,r),d=a.concat(d),j.scrollTo(0,r.offsetTop-69),j.finishPullDownLoad(),
w=d[0];
}
},
error:function(){}
}));
}
};
window.appmsg_album_info.size<=4&&(x.top=screen.height/2-(screen.height-window.innerHeight)+"px"),
j=new m(l,x),j.showLoading(),j.show(),o(l);
}
function d(){
var e=arguments.length<=0||void 0===arguments[0]?{}:arguments[0],t=document.getElementById("js_album_directory");
c.on(t,"click",function(){
j?j.show():l();
});
var a=document.getElementById("js_album_prev"),o=document.getElementById("js_album_next");
e.pre_article_link?c.on(a,"click",function(){
var t=void 0;
i({
ActionType:2,
SubActionType:1
}),n(12),t=e.pre_article_link.includes("#")?e.pre_article_link.replace("#",T+"&scene=189#"):e.pre_article_link+(T+"&scene=189"),
location.href=t.replace(/&amp;/g,"&");
}):a.parentNode.removeChild(a),e.next_article_link?c.on(o,"click",function(){
var t=void 0;
i({
ActionType:2,
SubActionType:2
}),n(13),t=e.next_article_link.includes("#")?e.next_article_link.replace("#",T+"&scene=189#"):e.next_article_link+(T+"&scene=189"),
location.href=t.replace(/&amp;/g,"&");
}):o.parentNode.removeChild(o);
}
e("page/appmsg_new/mod/album_read.css");
var c=e("biz_common/dom/event.js"),u=e("biz_common/dom/class.js"),m=e("pages/mod/bottom_modal.js"),_=e("biz_wap/utils/openUrl.js"),p=e("pages/utils.js"),g=e("biz_wap/utils/ajax.js"),b=e("common/comm_report.js"),f=e("common/utils.js"),w=e("biz_common/dom/offset.js"),v=e("biz_wap/utils/wapsdk.js"),h=document.getElementById("js_album_keep_read"),y={
link:window.appmsg_album_info.link,
albumId:window.appmsg_album_info.id,
msgid:window.mid,
idx:window.idx,
pageCount:10,
itemClassName:"album_read_directory_item"
},x={
MsgId:1*y.msgid,
ItemIdx:1*y.idx,
BizUin:window.biz,
Itemshowtype:1*window.item_show_type,
SessionId:window.sessionid,
EnterId:1*window.enterid,
Scene:1*window.source,
SubScene:1*window.subscene,
Albumid:y.albumId,
Albumtype:window.appmsg_album_info.type,
Title:window.appmsg_album_info.title
},j=void 0,T="&cur_album_id="+y.albumId;
return{
init:function(e){
h&&(e.pre_article_link||e.next_article_link)&&(document.getElementById("js_album_keep_read_title").innerHTML=window.appmsg_album_info.title,
document.getElementById("js_album_keep_read_size").innerHTML=window.appmsg_album_info.size,
document.getElementById("js_album_keep_read_pre_title").innerHTML=e.pre_article_title||"",
document.getElementById("js_album_keep_read_next_title").innerHTML=e.next_article_title||"",
h.style.display="block",d(e),t());
}
};
});define("appmsg/more_read.js",["biz_common/utils/string/html.js","biz_common/tmpl.js","biz_wap/utils/ajax.js","appmsg/more_read_tpl.html.js","biz_wap/utils/openUrl.js","biz_common/dom/event.js","biz_wap/utils/jsmonitor_report.js","common/utils.js"],function(n){
"use strict";
function i(n){
for(var i=r.getInnerHeight(),e=document.documentElement.clientWidth||window.innerWidth,t=document.body.scrollHeight||document.body.offsetHeight,s=document.body.scrollTop||document.documentElement.scrollTop,m=[],a=0;a<l.length;a++){
var w=[l[a].bizuin||window.biz||"",l[a].mid||"",l[a].idx||""].join("_");
m.push(w);
}
m=m.join("#");
var p=c[n.index].getBoundingClientRect(),h="fans_read_cnt="+l[n.index].fans_read_cnt,g={
act:n.action||0,
bizuin:window.biz||"",
msgid:window.mid||"",
idx:window.idx||"",
scene:window.source||"",
sub_scene:window.subscene||"",
get_a8_key_scene:window.ascene||"",
screen_height:i,
screen_width:e,
screen_num:Math.ceil(t/i),
action_screen_num:Math.ceil((p.top+p.height+s)/i),
start_time_ms:_,
action_time_ms:Date.now(),
more_msg:m,
a_bizuin:l[n.index].bizuin||window.biz||"",
a_msgid:l[n.index].mid||"",
a_idx:l[n.index].idx||"",
rank:n.index+1,
tip:h,
session_id:u
};
o({
url:"/mp/appmsgreport?action=more_read",
type:"POST",
data:g,
timeout:2e3,
async:!1,
mayAbort:!0
});
var b=1===n.action?4:5;
d.setSum(110809,b,1);
}
function e(){
if(l){
for(var n=0,t=r.getInnerHeight(),o=0;o<c.length;o++)if(c[o].dataset.show)n++;else{
var s=c[o].getBoundingClientRect();
s.top+s.height<t&&(c[o].dataset.show=1,i({
action:1,
index:o
}));
}
n>=c.length&&a.off(window,"scroll",e);
}
}
n("biz_common/utils/string/html.js");
var t=n("biz_common/tmpl.js"),o=n("biz_wap/utils/ajax.js"),s=n("appmsg/more_read_tpl.html.js"),m=n("biz_wap/utils/openUrl.js"),a=n("biz_common/dom/event.js"),d=n("biz_wap/utils/jsmonitor_report.js"),r=n("common/utils.js"),l=null,c=null,_=Date.now(),u=""+_+"_"+Math.random().toString(36).substring(2);
return a.on(window,"scroll",e),function(n,e){
l=e,n.innerHTML=t.tmpl(s,{
list:l
},!1),c=n.getElementsByClassName("more_read_link");
for(var o=0;o<c.length;o++)a.on(c[o],"click",function(n){
return function(){
window.__second_open__?m.openUrlWithExtraWebview(l[n].link.htmlDecode()):window.location.href=l[n].link.htmlDecode(),
i({
action:2,
index:n
});
};
}(o));
n.style.display="";
};
});var _extends=Object.assign||function(e){
for(var t=1;t<arguments.length;t++){
var n=arguments[t];
for(var o in n)Object.prototype.hasOwnProperty.call(n,o)&&(e[o]=n[o]);
}
return e;
};
define("appmsg/comment.js",["biz_wap/ui/weui.js","biz_common/utils/string/html.js","biz_common/utils/wxgspeedsdk.js","appmsg/comment_report.js","biz_wap/utils/device.js","biz_common/utils/url/parse.js","biz_wap/jsapi/core.js","common/utils.js","biz_common/dom/event.js","biz_wap/utils/ajax.js","biz_common/tmpl.js","biz_wap/utils/fakehash.js","appmsg/log.js","appmsg/comment/comment_tpl.html.js","appmsg/comment/comment_write.html.js","appmsg/comment/comment_write_old.html.js","pages/utils.js","biz_wap/utils/mmversion.js","common/comm_report.js","appmsg/set_font_size.js","biz_wap/utils/jsmonitor_report.js","common/keyboard.js","appmsg/comment/comment_write_dialog/comment_write_dialog.js","appmsg/comment/comment_list/comment_list.js","appmsg/comment/comment_dialog/comment_dialog.js","appmsg/comment/comment_input/comment_input.js","appmsg/comment/comment_length.js","pages/scrollY.js","appmsg/comment/comment_report.js","pages_new/common_share/video/store.js"],function(e){
"use strict";
e("biz_wap/ui/weui.js"),e("biz_common/utils/string/html.js");
var t=e("biz_common/utils/wxgspeedsdk.js"),n=e("appmsg/comment_report.js"),o=e("biz_wap/utils/device.js"),i=e("biz_common/utils/url/parse.js"),m=e("biz_wap/jsapi/core.js"),s=e("common/utils.js"),a=e("biz_common/dom/event.js"),c=e("biz_wap/utils/ajax.js"),d=e("biz_common/tmpl.js"),r=e("biz_wap/utils/fakehash.js"),l=e("appmsg/log.js"),p=e("appmsg/comment/comment_tpl.html.js"),_=e("appmsg/comment/comment_write.html.js"),u=e("appmsg/comment/comment_write_old.html.js"),w=e("pages/utils.js"),y=e("biz_wap/utils/mmversion.js"),g=e("common/comm_report.js"),f=e("appmsg/set_font_size.js"),I=e("biz_wap/utils/jsmonitor_report.js"),C=e("common/keyboard.js"),b=e("appmsg/comment/comment_write_dialog/comment_write_dialog.js"),j=e("appmsg/comment/comment_list/comment_list.js"),h=e("appmsg/comment/comment_dialog/comment_dialog.js"),v=e("appmsg/comment/comment_input/comment_input.js"),k=e("appmsg/comment/comment_length.js"),L=e("pages/scrollY.js"),D=e("appmsg/comment/comment_report.js"),R=D.report22214,x=!window.isPaySubscribe||window.isPaySubscribe&&window.isPaid,F=s.getInnerHeight(),T=k.getLimit("comment"),S=0;
try{
S=1*window.atob(window.biz);
}catch(P){}
var z={
BizUin:S,
BizUinStr:window.biz||"",
AppMsgId:window.parseInt(window.mid,10)||0,
ItemIdx:window.parseInt(window.idx,10)||0,
ItemShowType:window.parseInt(window.item_show_type,10)||0,
SessionIdStr:window.sessionid||"",
EnterId:window.parseInt(window.enterid,10)||0,
Scene:window.parseInt(window.source,10)||0,
SubScene:window.parseInt(window.subscene,10)||0
},E={
bizuin:S,
msgid:window.parseInt(window.mid,10)||0,
itemidx:window.parseInt(window.idx,10)||0,
scene:window.parseInt(window.source,10)||0
},A=!1,B=void 0,M=void 0,N=void 0,O=void 0,W=!1,H=[],U={},K=0,G=Date.now(),V=[],q=null,Y=!1,J=!1,Q=!1,X={
commentCount:"",
scrollCount:0,
nickName:"我",
headImg:"",
offset:0,
commentId:window.comment_id,
onlyFansCanComment:0,
onlyFansDaysCanComment:0,
isFans:0,
isFansDays:0,
replyFlag:0,
reportData:{
scene:0,
idkey:"",
moreList:27,
repeatList:25,
errList:18,
handleList:26,
addCommentErr:19,
errComment:18,
repeatContent:24,
repeatContentID:23
},
contentIDs:[],
canC2CReply:!1,
cmtDialog:null,
cmtList:null,
myList:null
},Z=100,$=location.href,et=y.is_wxwork,tt=o.os.pc&&!et,nt="comment_editing",ot="my_comment_empty_data",it=navigator.userAgent.indexOf("MicroMessenger")>-1,mt=w.getId("js_cmt_area"),st=$.indexOf("vconsole=1")>0||document.cookie&&document.cookie.indexOf("vconsole_open=1")>-1,at=w.getId("activity-name")||"";
at&&(at=w.trim(at.innerText)||""),window.pageCommentReportData&&window.pageCommentReportData.idkey&&(st&&console.log("init reportData"),
X.reportData=window.pageCommentReportData),"undefined"!=typeof window.comment_id?X.commentId=window.comment_id:window.cgiData&&"undefined"!=typeof window.cgiData.comment_id&&(X.commentId=window.cgiData.comment_id),
it||(mt&&(mt.style.display="none"),X.commentId=0),st&&console.info("[图文评论] 评论ID:",X.commentId);
var ct=function(){
return y.isWechat?y.isInMiniProgram?0:C.canUseKeyboard?2:(y.isIOS||y.isAndroid)&&(y.gtVersion("7.0.8")||s.isNativePage())?2:0:0;
}(),dt=function(e,t){
e&&(e.style.display=t||"block");
},rt=function(e){
e&&(e.style.display="none");
},lt=function(e){
if(!e)return!1;
var t=s.getScrollTop(),n=e.offsetTop;
return t+F>n&&n>=t;
},pt=function(e,n){
Math.random()<.999||(t.saveSpeeds({
uin:window.uin,
pid:"https:"===window.location.protocol?18:9,
speeds:[{
sid:29,
time:e
},{
sid:30,
time:n
}]
}),t.send());
},_t=function(e){
var t=arguments.length<=1||void 0===arguments[1]?"":arguments[1];
if("undefined"!=typeof e)if(X.reportData.idkey)I.setSum(X.reportData.idkey,e,1);else{
var n=new Image,o=Math.random();
n.src="/mp/jsreport?key="+e+"&content="+t+"&r="+o;
}
},ut=function(){
Array.prototype.forEach.call(w.getByClass("js_more_reply"),function(e){
var t=e.dataset.myId;
-1===V.indexOf(t)&&lt(e)&&(g.report(19462,_extends({
PersonalCommentId:parseInt(t,10)||0,
CommentId:parseInt(X.commentId,10)||0,
actiontype:1,
wording:"余下N条",
number:parseInt(e.dataset.num,10)||0,
devicetype:tt?1:2
},E)),V.push(t));
});
},wt=function(){
2>K||[X.myList.getItemList(),X.cmtList.getItemList()].forEach(function(e,t){
var n=X[t?"cmtList":"myList"];
e.some(function(e){
if(!e.isExposed){
var o=e.getBoundingClientRect(),i=.5*o.height;
if(o.bottom>i&&o.top<F-i){
e.isExposed=!0;
var m=e.dataset,s={
PersonalCommentId:1*m.myId,
ReplyId:0,
IsPopup:0,
IsReplyOther:0,
CommentReplyType:t?1:2
};
if(m.replyId){
var a=n.getData({
type:"reply",
contentId:m.contentId,
replyId:1*m.replyId
});
s.ReplyId=a.reply_id,s.IsReplyOther=a.to_nick_name&&a.to_content?1:0;
}
R(s);
}else if(o.top>=F-i)return!0;
}
return!1;
});
});
},yt=function(){
M||(M=!0,new n({
comment_id:X.commentId,
appmsgid:window.appmsgid,
idx:window.idx,
item_show_type:window.item_show_type||0,
biz:window.biz
}),ut());
},gt=function Bt(){
try{
lt(U.loading)&&W&&(I.setLogs({
id:28307,
key:45,
value:1,
lc:1,
log0:""
}),a.off(window,"scroll",Bt));
}catch(e){
console.error(e);
}
},ft=function(){
X.myList.showAll(),wt(),U.mylistFolder&&(U.mylistFolder.parentNode.removeChild(U.mylistFolder),
U.mylistFolder=null);
},It=function(){
var e=!0,t=!0,n=!1,o=null,i=null,m=null,s=function(e){
e&&window.scrollTo(0,e.getBoundingClientRect().top+w.getScrollTop()-6);
};
return function(a,c){
if(e&&window.goContentId&&!c&&a)if(!window.onload_endtime||new Date-window.onload_endtime<1e3){
if(J&&(null===o&&(o=null!==X.myList.getData({
contentId:window.goContentId
})),o))return void("none"!==mt.style.display&&(ft(),s(""!==window.goReplyId?X.myList.getReply(window.goContentId,1*window.goReplyId):X.myList.getComment(window.goContentId)),
e=!1));
Y&&(null===i&&(m=X.cmtList.getData({
contentId:window.goContentId
}),i=null!==m),i&&(t&&""!==window.goReplyId&&(m.reply_new.reply_total_cnt&&m.reply_new.reply_total_cnt!==m.reply_new.reply_list.length?X.cmtDialog.showDialog(X.cmtList.getData({
contentId:window.goContentId
}),1*window.goReplyId):n=!0,t=!1),J&&(s(n?X.cmtList.getReply(window.goContentId,1*window.goReplyId):X.cmtList.getComment(window.goContentId)),
e=!1)));
}else e=!1;
};
}(),Ct=function(e,t){
X.onlyFansDaysCanComment&&0===X.isFansDays?((t||e).innerHTML="作者已设置关注7天后才可留言",dt(e,"block")):X.onlyFansCanComment&&0===X.isFans?((t||e).innerHTML="作者已设置关注后才可以留言",
dt(e,"block")):x&&(tt?(dt(U.commentPC),dt(U.inputPC)):dt(U.addBtn,"flex"));
},bt=function(e){
var n=Date.now(),o=e.resp,i=e.loadTime,m=e.forceRefresh,s=e.notFirstRender;
if(X.onlyFansCanComment=o.only_fans_can_comment,X.isFans=o.is_fans,window.isPaySubscribe&&!window.isPaid&&(X.canC2CReply=!1),
X.replyFlag=X.canC2CReply?o.reply_flag:0,_t(X.reportData.handleList,encodeURIComponent(JSON.stringify({
comment_id:X.commentId,
offset:X.offset,
url:$
}))),1!==o.enabled?(mt&&(mt.style.display="none"),o.elected_comment=[],o.elected_comment_total_cnt=0):mt&&(mt.style.display="block"),
0===X.offset){
X.headImg=o.logo_url,X.nickName=o.nick_name,m&&(X.contentIDs=[]);
var a=o.elected_comment;
a&&a.length?(dt(U.main),X.cmtList.render(a),!et&&Ct(w.getId("js_cmt_nofans1")),o.elected_comment_total_cnt<=10&&dt(U.statement),
N||"5"!==window.item_show_type||(N=!0,Math.random()<.1&&(t.saveSpeeds({
uin:window.uin,
pid:675,
speeds:[{
sid:27,
time:Date.now()-window.logs.pagetime.page_begin
}]
}),t.send())),Y=!0,It(m,s)):(rt(U.main),!et&&Ct(w.getId("js_cmt_nofans2"),w.getId("js_cmt_nofans2_inner")));
var c=mt.getBoundingClientRect().y;
location.href.indexOf("scrolltodown")>-1&&c&&window.scrollTo(0,c-25);
}else{
var a=o.elected_comment;
a&&a.length&&X.cmtList.add({
data:a
});
}
0===o.elected_comment_total_cnt?(X.offset=-1,rt(U.loading),rt(U.statement)):X.offset+Z>=o.elected_comment_total_cnt?(X.offset=-1,
rt(U.loading),dt(U.statement)):X.offset+=o.elected_comment.length,window.ipados13_font_scale&&f.setFontSize(U.main,window.ipados13_font_scale/100),
yt(),w.setTwoTabHeight("js_comment_content"),i&&pt(i,Date.now()-n);
},jt=function(e){
if(X.commentId=window.comment_id,0!==Number(X.commentId)){
var t=e.notFirstRender,n=e.forceRefresh,o=e.cb;
n=n===!0,n&&(X.offset=0);
var m=s.getScrollTop(),d=document.documentElement.scrollHeight;
if(!(W||-1===X.offset||X.offset>0&&d-m-F>500)){
if("number"==typeof X.commentCount&&0===X.commentCount&&!n)return void bt({
resp:{
enabled:1,
elected_comment:[],
elected_comment_total_cnt:0,
my_comment:[],
only_fans_can_comment:X.onlyFansCanComment,
is_fans:X.isFans,
logo_url:X.headImg,
nick_name:X.nickName
}
});
var r=i.join("/mp/appmsg_comment",{
action:"getcomment",
scene:X.reportData.scene,
appmsgid:window.appmsgid,
idx:window.idx,
comment_id:X.commentId,
offset:X.offset,
limit:Z,
send_time:window.send_time
},!0),p=+new Date;
W=!0,dt(U.loading),gt();
try{
X.scrollCount++,n&&(H=[]),X.scrollCount>1&&!n&&_t(X.reportData.moreList,encodeURIComponent(r)),
H.indexOf(r)>-1&&_t(X.reportData.repeatList,encodeURIComponent(r)),H.push(r);
}catch(_){
console.error(_);
}
st&&console.info("[图文评论] 开始请求评论数据:",r),l("[Appmsg comment] start get comment data, url:"+r),
c({
url:r,
dataType:"json",
success:function(e){
var i=e.base_resp&&e.base_resp.ret;
0===i?o&&o({
resp:e,
forceRefresh:n,
notFirstRender:t,
loadTime:Date.now()-p
}):_t(X.reportData.errList,"type:resperr;url:"+encodeURIComponent(r)+";ret="+i),
l("[Appmsg comment] get comment success");
},
error:function(){
_t(X.reportData.errList,"type:ajaxerr;url:"+encodeURIComponent(r)),l("[Appmsg comment] get comment ajax error");
},
complete:function(){
W=!1,rt(U.loading),a.off(window,"scroll",gt);
}
});
}
}
},ht=function(e){
var t=j.validContent(e);
return t.valid&&tt&&(t.content=q.value),t;
},vt=!1,kt=function(e){
var t=e.content,n=e.successBegin,o=e.successEnd,m=e.fail,s=e.complete;
vt||!function(){
vt=!0,B!==t&&(G=Date.now());
var e=i.join("/mp/appmsg_comment",{
action:"addcomment",
scene:X.reportData.scene,
appmsgid:window.appmsgid,
idx:window.idx,
comment_id:X.commentId,
sn:window.sn
},!0);
c({
url:e,
data:{
content:t,
title:at,
head_img:X.headImg,
nickname:X.nickName,
client_id:G
},
type:"POST",
dataType:"json",
success:function(i){
switch("function"==typeof n&&n(),+i.ret){
case 0:
window.weui.toast("已留言",750);
var s={
content:t,
nick_name:X.nickName,
create_time:Date.now()/1e3|0,
is_elected:0,
logo_url:X.headImg,
like_status:0,
like_num_format:0,
like_num:0,
is_from_friend:0,
is_from_me:1,
my_id:i.my_id,
content_id:i.content_id,
reply_new:{
reply_list:[]
}
};
return dt(U.mylistContainer),dt(U.mylistFooter),X.myList.add({
data:[s],
mode:"unshift"
}),ft(),void("function"==typeof o&&o());

case-6:
window.weui.alert("你留言的太频繁了，休息一下吧");
break;

case-7:
window.weui.alert("你还未关注该公众号，不能参与留言");
break;

case-10:
window.weui.alert("字数不能多于"+T+"个");
break;

case-15:
window.weui.alert("留言已关闭");
break;

case-18:
window.weui.alert("你在此文章的留言次数已达上限");
break;

default:
B=t,window.weui.alert("系统错误，请重试");
}
_t(X.reportData.addCommentErr,"type:resperr;url:"+encodeURIComponent(e)+";ret="+i.ret),
"function"==typeof m&&m();
},
error:function(t){
console.log(t),_t(X.reportData.addCommentErr,"type:ajaxerr;url:"+encodeURIComponent(e)),
"function"==typeof m&&m();
},
complete:function(){
vt=!1,"function"==typeof s&&s();
}
});
}();
},Lt=function(){
var e=q.getSubmit(),t=q.getInput();
if(e.disabled!==!0){
var n=ht(tt?q.value:t.value),o=n.valid,i=n.content;
o&&(e.disabled=!0,kt({
content:i,
successBegin:function(){
!tt&&q.hideEmotionPannel();
},
successEnd:function(){
tt?(q.hide(),dt(U.inputPC)):t.value="";
},
complete:function(){
""!==t.value&&(e.disabled=!1);
}
}));
}
},Dt=function(){
var e=i.join("/mp/appmsg_comment",{
action:"getmycomment",
scene:X.reportData.scene,
appmsgid:window.appmsgid,
idx:window.idx,
comment_id:X.commentId
},!0);
0===K&&(K=1,Array.prototype.forEach.call(U.myCmtLoading,dt),c({
url:e,
dataType:"json",
success:function(t){
var n=t.base_resp&&t.base_resp.ret;
if(0===n){
K=2;
var o=t.my_comment;
o&&o.length&&(dt(U.mylistContainer),dt(U.mylistFooter),X.myList.render(o),X.myList.getIsOversize()&&dt(U.mylistFolder)),
J=!0,It(!0,!1);
}else K=0,_t(X.reportData.errComment,"type:resperr;url:"+encodeURIComponent(e)+";ret="+n);
},
error:function(){
K=0,_t(X.reportData.errComment,"type:ajaxerr;url:"+encodeURIComponent(e));
},
complete:function(){
Array.prototype.forEach.call(U.myCmtLoading,rt);
}
}));
},Rt=function(){
var e="discuss_form_editing";
return{
enterEditing:function(){
Q=!0,C.canUseCancel&&(U.addBtn.classList.add(e),U.cmtWritePlaceholder.innerHTML="正在输入...");
},
leaveEditing:function(){
Q&&(Q=!1,C.canUseCancel&&(U.addBtn.classList.remove(e),U.cmtWritePlaceholder.innerHTML="写下你的留言",
y.isAndroid&&(document.body.style.marginBottom="0px")));
}
};
}(),xt=Rt.leaveEditing,Ft=function(){
switch(ct){
case 2:
X.cmtWriteDialog.show();
break;

case 1:
m.invoke("handleMPPageAction",{
action:"writeComment",
title:at,
comment_id:X.commentId,
style:"white"
});
}
return ct;
},Tt=function(e){
A=!0,O=s.getScrollTop(),!tt&&rt(U.article),X.myList.changeContainer(U.mylistOld),
ft(),dt(U.mine),document.body.classList[X.myList.count?"remove":"add"](ot),window.__second_open__&&o.os.ios&&dt(U.fakebar),
window.scrollTo(0,0),Dt(),!e&&setTimeout(function(){
return q.focus();
},3);
},St=function(){
A=!1,X.myList.changeContainer(U.mylist),rt(U.mine),dt(U.article),window.scrollTo(0,O),
q.blur(),document.body.classList.remove(nt),document.body.classList.remove(ot);
},Pt=function(t,n){
if(1*window.item_show_type===5){
var i=e("pages_new/common_share/video/store.js");
i.dispatch("mp-video-player/auto-next-plugin/cancelAutoNextWhenTipsShowed",6);
}
switch(t&&t.preventDefault(),Ft()){
case 2:
g.report(19048,_extends({
EventType:1,
IsFans:X.isFans,
CommentPageType:3
},z));
break;

case 1:
g.report(19048,_extends({
EventType:1,
IsFans:X.isFans,
CommentPageType:2
},z));
break;

case 0:
default:
if(s.isNativePage()||document.body.classList.add(nt),n)return st&&console.log("FakeHash on comment"),
void Tt();
t.preventDefault(),window.__second_open__&&o.os.ios?Tt():(st&&console.log("push comment"),
r.push("comment")),g.report(19048,_extends({
EventType:1,
IsFans:X.isFans,
CommentPageType:1
},z));
}
},zt=function(){
a.tap(U.mylistFolder,ft),a.on(window,"scroll",gt),a.on(window,"scroll",ut),a.bindVisibilityChangeEvt(function(e){
e&&!lt(U.cmtContainer)&&jt({
notFirstRender:!0,
forceRefresh:!0,
cb:bt
});
});
var e=function t(){
U.mylistFolder?lt(U.mylistFolder)&&(g.report(19462,_extends({
CommentId:parseInt(X.commentId,10)||0,
actiontype:1,
wording:"展开全部留言",
number:X.myList.count,
devicetype:1
},E)),a.off(window,"scroll",t)):a.off(window,"scroll",t);
};
if(a.on(window,"scroll",e),e(),s.bindDebounceScrollEvent(wt),tt)a.tap(U.inputPC,function(){
rt(U.inputPC),q.show(U.inputPC);
});else switch(a.tap(U.cmtWrite,Pt),ct){
case 2:
y.isAndroid&&C.canUseCancel&&C.onGetKeyboardHeight(function(e){
if(Q){
var t=e+55,n=F-U.cmtWrite.getBoundingClientRect().top-t,o={
y:s.getScrollTop()-n
};
Math.abs(n)<150?o.speed=300:o.duration=.3,document.body.style.marginBottom=t+"px",
L(o);
}
});
break;

case 1:
s.listenMpPageAction(function(e){
"deleteComment"===e.action&&X.cmtList.remove({
myId:e.personal_comment_id
}),"deleteCommentReply"===e.action&&X.cmtList.remove({
type:"reply",
myId:e.personal_comment_id,
replyId:e.replyId
}),"praiseComment"===e.action&&(e.reply_id&&0!==e.reply_id?(X.cmtList.setLikeInfo({
type:"reply",
myId:e.personal_comment_id,
replyId:e.reply_id,
likeStatus:e.is_like
}),X.cmtDialog.setReplyLikeInfo({
myId:e.personal_comment_id,
replyId:e.reply_id,
likeStatus:e.is_like
})):X.cmtList.setLikeInfo({
myId:e.personal_comment_id,
likeStatus:e.is_like
}));
});
break;

case 0:
U.goback&&a.on(U.goback,"click",function(e){
e.preventDefault(),St(),rt(U.fakebar);
}),o.os.ios&&window.__second_open__&&!function(){
var e=null,t=null;
a.on(window,"orientationchange",function(){
"none"!==U.fakebar.style.display&&(clearTimeout(e),e=setTimeout(function(){
window.innerWidth!==parseFloat(getComputedStyle(U.fakebar).width)&&(clearTimeout(t),
U.mine.style.height=F+"px",window.scrollBy&&window.scrollBy(0,1),t=setTimeout(function(){
window.scrollBy&&window.scrollBy(0,-1),U.mine.style.height="";
},100));
},50));
});
}();
}
},Et=function(e){
if(X.canC2CReply=0!==(256&e.test_flag),X.onlyFansCanComment=e.only_fans_can_comment,
X.nickName=e.nick_name,X.isFans=e.is_fans,X.headImg=e.logo_url,X.commentCount=e.comment_count,
X.onlyFansDaysCanComment=e.only_fans_days_can_comment,X.isFansDays=e.is_fans_days,
window._has_comment=!0,console.log("inwechat",it,"commentid",X.commentId),!it||0===Number(X.commentId))return void(window._has_comment=!1);
var t=w.getId("js_cmt_container");
mt&&(t.insertAdjacentHTML("afterbegin",d.tmpl(_,{
deviceIsPc:tt
})),X.cmtWriteDialog=new b({
canC2CReply:X.canC2CReply,
onSubmit:function(e){
var t=ht(e),n=t.valid,o=t.content;
n&&(X.cmtWriteDialog.disableSubmit(),kt({
content:o,
successEnd:function(){
X.cmtWriteDialog.hide(),X.cmtWriteDialog.setInputValue("");
},
fail:function(){
X.cmtWriteDialog.enableSubmit();
}
}));
}
}),mt.innerHTML=d.tmpl(p,{
isWxWork:et
})),tt?document.body.classList.add("pages_skin_pc"):0===ct&&document.body.insertAdjacentHTML("beforeend",d.tmpl(u,{
textPageTitle:1*window.item_show_type===10?w.getId("js_text_content").innerHTML.replace(/<(\/?)(?=((a(\s|>))|(\/a))).*?>/g,""):window.msg_title.html(1)
},!1));
var n=w.getId("js_cmt_write");
U={
article:w.getId("js_article"),
mine:w.getId("js_cmt_mine"),
main:w.getId("js_cmt_main"),
input:w.getId("js_cmt_input"),
goback:w.getId("js_cmt_goback"),
list:w.getId("js_cmt_list"),
mylistContainer:w.getId("js_my_list_container"),
mylist:w.getId("js_my_list"),
mylistFolder:w.getId("js_my_list_folder"),
mylistFooter:w.getId("js_my_list_footer"),
mylistOld:w.getId("js_my_list_old"),
morelist:w.getId("js_cmt_morelist"),
loading:w.getId("js_cmt_loading"),
fakebar:w.getId("js_fake_bar"),
statement:w.getId("js_cmt_statement"),
myCmtLoading:w.qsAll(".js_mycmt_loading"),
cmtWrite:n,
cmtWritePlaceholder:w.qs(".js_cmt_write_placeholder",n),
cmtContainer:t,
commentPC:w.getId("js_comment_pc"),
inputPC:w.getId("js_cmt_input_pc"),
containerPC:w.getId("js_cmt_container_pc"),
addbtnPC:w.getId("js_cmt_addbtn_pc"),
emotionSwitchPC:w.getId("js_emotion_wrp_pc"),
inputHolder:w.getId("js_cmt_input_holder"),
addBtn:w.getId("js_cmt_addbtn"),
updateDialog:w.getId("js_update_dialog"),
updateCancel:w.getId("js_update_cancel"),
updateConfirm:w.getId("js_update_confirm"),
deleteReplyPanel:w.getId("js_delete_reply_panel"),
deleteReplyConfirm:w.getId("js_delete_reply_confirm"),
deleteReplyCancel:w.getId("js_delete_reply_cancel"),
cmtDialog:w.getId("js_cmt_dialog")
},X.cmtDialog=new h({
globalData:X,
canAddComment:x,
onGetReplyList:function(e,t){
t.filter(function(e){
return 2===e.is_from;
}).forEach(function(t){
X.cmtList.setLikeInfo({
type:"reply",
contentId:e,
replyId:t.reply_id,
likeStatus:t.reply_like_status,
likeNum:t.reply_like_num
});
});
}
}),X.cmtList=new j({
globalData:X,
container:U.list,
type:"elected",
canAddComment:x,
onPraise:function(e,t,n,o,i){
!X.canC2CReply&&X.myList.setLikeInfo({
type:e,
contentId:t,
replyId:n,
likeStatus:o,
likeNum:i
}),X.cmtDialog.setReplyLikeInfo({
contentId:t,
replyId:n,
likeStatus:o,
likeNum:i
});
},
onRender:function(){
wt();
},
onAdd:function(e,t,n,o,i){
!X.canC2CReply&&X.myList.add({
data:e,
mode:t,
type:n,
contentId:o,
pos:i
}),wt();
},
onRemove:function(e,t,n){
!X.canC2CReply&&X.myList.remove({
type:e,
contentId:t,
replyId:n.reply_id
}),wt();
},
onEmpty:function(e){
"comment"===e&&(rt(U.main),rt(U.statement));
},
beforeShowKeyboard:function(){
xt();
}
}),X.myList=new j({
globalData:X,
container:U.mylist,
type:"mine",
canAddComment:x,
onPraise:function(e,t,n,o,i){
!X.canC2CReply&&X.cmtList.setLikeInfo({
type:e,
contentId:t,
replyId:n,
likeStatus:o,
likeNum:i
});
},
onRender:function(){
wt();
},
onAdd:function(e,t,n,o,i){
!X.canC2CReply&&"reply"===n&&X.cmtList.add({
data:e,
mode:t,
type:n,
contentId:o,
pos:i
}),wt(),ft();
},
onRemove:function(e,t,n){
U.mylistFolder&&X.myList.showTopItems()&&ft(),!X.canC2CReply&&X.cmtList.remove({
type:e,
contentId:t,
replyId:n.reply_id
}),wt();
},
onEmpty:function(){
rt(U.mylistContainer),rt(U.mylistFooter),A&&document.body.classList.add(ot);
},
beforeShowKeyboard:function(){
xt();
}
}),window.cl=X.cmtList,window.ml=X.myList,0===ct&&window.__second_open__&&o.os.ios&&(U.mine.style.marginBottom=getComputedStyle(U.fakebar).height),
!e.notAutoGetComment&&jt({
forceRefresh:!0,
cb:bt
}),"1"===i.getQuery("js_my_comment")&&(1===ct?Ft():0!==ct||tt||Tt(!0)),Dt(),zt(),
tt?q=new v({
placeholder:"留言被公众号精选后，将对所有人可见",
submitText:"留言",
length:T,
onSubmit:Lt,
onHide:function(){
dt(U.inputPC);
}
}):0===ct&&(q=new v({
placeholder:"留言被公众号精选后，将对所有人可见",
submitText:"留言",
length:T,
onSubmit:Lt,
onClick:function(){
window.__second_open__&&rt(U.fakebar);
},
onBlur:function(){
"none"!==U.mine.style.display&&dt(U.fakebar);
}
}),q.show(w.getId("js_comment_input_old"),{
renderType:"append"
}));
},At=function(){
tt||0!==ct||(r.on("comment",function(){
Pt(null,!0);
}),r.on("article",function(){
st&&console.log("FakeHash on article"),St();
}),r.on(function(e){
"comment"===e&&St();
}));
};
return At(),{
initComment:Et,
getCommentData:jt,
renderComment:bt
};
});define("appmsg/like_and_share.js",["biz_common/dom/event.js","biz_common/dom/class.js","biz_wap/jsapi/core.js","pages/utils.js","appmsg/retry_ajax.js","appmsg/set_font_size.js","common/comm_report.js","appmsg/related_article.js","appmsg/like_profile.js","common/utils.js","biz_wap/utils/device.js","biz_wap/utils/mmversion.js","appmsg/appmsg_report.js"],function(e,i,o,n){
"use strict";
var t=e("biz_common/dom/event.js"),s=e("biz_common/dom/class.js"),r=e("biz_wap/jsapi/core.js"),m=e("pages/utils.js"),a=m.formatReadNum,l=e("appmsg/retry_ajax.js"),d=e("appmsg/set_font_size.js"),p=e("common/comm_report.js"),w=e("appmsg/related_article.js"),c=e("appmsg/like_profile.js"),_=e("common/utils.js"),u=e("biz_wap/utils/device.js"),k=e("biz_wap/utils/mmversion.js"),g=e("appmsg/appmsg_report.js"),h=function(e){
return document.getElementById(e);
},j=function(e){
e.style.display="block";
},f={
likeNum:0,
isLike:0,
likeDom:h("like_old"),
likeNumDom:h("likeNum_old"),
shareDom:h("js_bottom_share"),
collectDom:h("js_bottom_collect"),
oprRightDom:h("js_bottom_opr_right"),
shareBottomBtn:h("js_bottom_share_btn"),
likeBottomBtn:h("js_bottom_zan_btn"),
similarZanCard:h("js_similar_video_card"),
overflowFontScale:1
},v=function(e){
c&&c.renderLikeProfile&&c.renderLikeProfile(e);
},D=function(){
var e=0;
try{
e=1*window.atob(window.biz);
}catch(i){}
var o={
BizUin:e,
BizUinStr:window.biz||"",
AppMsgId:window.parseInt(window.mid,10)||0,
ItemIdx:window.parseInt(window.idx,10)||0,
ItemShowType:window.parseInt(window.item_show_type,10)||0,
SessionIdStr:window.sessionid||"",
EnterId:window.parseInt(window.enterid,10)||0,
Scene:window.parseInt(window.source,10)||0,
SubScene:window.parseInt(window.subscene,10)||0,
EventType:4
};
p.report(19048,o);
},b=function(){
setTimeout(function(){
s.removeClass(f.oprRightDom,"sns_opr_overflow");
var e=f.oprRightDom.querySelectorAll(".js_media_tool_meta"),i=h("js_toobar3").getBoundingClientRect().width,o=0;
if(e&&e.length){
h("js_like_wording").textContent="在看",h("js_parise_wording").textContent="赞";
for(var n=0;n<e.length;n++)o+=e[n].getBoundingClientRect().width;
if(console.log("benchmarkWidth",i,o),e.length>1&&(i-o)/(e.length-1)<20){
h("js_like_wording").textContent="",h("js_parise_wording").textContent="";
for(var t=0,n=0;n<e.length;n++)t+=e[n].getBoundingClientRect().width;
(i-t)/(e.length-1)<20&&s.addClass(f.oprRightDom,"sns_opr_overflow");
}
}
},50);
},y=function(){
s.addClass(f.likeDom,"praised"),f.likeNum++;
var e=f.likeNumDom.innerHTML;
("10万+"!==e||"100k+"!==e)&&(f.likeNumDom.innerHTML=a(f.likeNum)),f.likeNumDom.style.display="",
w&&w.render&&w.render("praise"),v(["like"]);
},B=function(){
s.removeClass(f.likeDom,"praised"),f.likeNum--;
var e=f.likeNumDom.innerHTML;
f.likeNum>=0&&"10万+"!==e&&"100k+"!==e&&(f.likeNumDom.innerHTML=a(f.likeNum)),0===f.likeNum&&(f.likeNumDom.style.display="none");
},N=[],z=function(e){
"function"==typeof e&&N.push(e);
},L=function(e){
f.isLike=f.isLike?0:1,f.isLike?y():B(),l({
url:"/mp/appmsg_like?__biz="+window.biz+"&mid="+window.mid+"&idx="+window.idx+"&like="+f.isLike+"&f=json&appmsgid="+window.appmsgid+"&itemidx="+window.itemidx,
data:{
scene:window.source,
appmsg_like_type:1,
item_show_type:parseInt(window.item_show_type,10),
client_version:window.clientversion,
is_temp_url:window.is_temp_url||0,
style:e||0,
exptype:window.exptype||"",
expsessionid:window.expsessionid||""
},
type:"POST"
}),N.forEach(function(e){
e(f.isLike,f.likeNum);
});
},I=function(e){
return f.likeBottomBtn&&f.likeBottomBtn.disabled===!0?void 0:window.is_temp_url?void("5"!==window.item_show_type||!_.isNativePage()||u.os.pc?window.weui.alert("预览状态下无法操作"):n("预览状态下无法操作")):void L(e);
};
t.on(f.likeDom,"click",function(){
return I();
}),t.on(f.shareDom,"click",function(){
f.shareBottomBtn&&f.shareBottomBtn.disabled===!0||(D(),r.invoke("handleMPPageAction",{
action:"share"
}),v(["share"]));
}),t.on(f.collectDom,"click",function(){
r.invoke("handleMPPageAction",{
action:"doFavorite"
}),w&&w.render&&w.render("favorite"),v(["collect"]),g.shareReport({
link:window.msg_link||window.cgiData&&window.cgiData.msg_link,
action_type:24,
share_source:2
});
});
var x=function(){
b(),d.onFontScaleChange(b),window.addEventListener("resize",b);
},C=function(e){
var i=e.shareShow,o=e.likeShow,n=e.likeNum,t=e.isLike,r=e.shareGray,m=e.likeGray;
f.likeNum=n,f.isLike=t,i&&f.shareDom&&(u.os.pc?u.os.windows&&k.getInner()>="63010000"&&j(f.shareDom):j(f.shareDom)),
r&&f.shareBottomBtn&&(f.shareBottomBtn.disabled=!0),o&&f.likeDom&&j(f.likeDom),m&&f.likeBottomBtn&&(f.likeBottomBtn.disabled=!0),
o&&f.likeNumDom&&0!==n&&(f.likeNumDom.innerHTML=a(f.likeNum),f.likeNumDom.style.display="",
t&&s.addClass(f.likeDom,"praised")),(k.isWechat&&(u.os.iphone&&k.getInner()>"17001000"||u.os.android&&k.getInner()>"27001300")||u.os.windows&&k.getInner()>="63010000")&&j(f.collectDom),
x(),N.forEach(function(e){
e(f.isLike,f.likeNum);
});
};
return{
initLikeShareDom:C,
triggerLike:I,
onLikeChange:z,
renderProfile:v
};
});define("appmsg/like.js",["biz_common/dom/event.js","biz_common/dom/class.js","biz_wap/utils/ajax.js","biz_common/base64.js","biz_wap/utils/jsmonitor_report.js","appmsg/log.js","complain/tips.js","appmsg/retry_ajax.js","biz_wap/jsapi/core.js","biz_wap/utils/mmversion.js","common/utils.js","appmsg/loading.js","appmsg/like_and_share.js","biz_wap/utils/device.js","appmsg/pay_report_utils.js","pages/utils.js","appmsg/related_article.js"],function(require,exports,module,alert){
"use strict";
function qs(e){
return document.getElementById(e);
}
function showAppToast(e,i){
JSAPI.invoke("handleMPPageAction",{
action:"showToast",
wording:e||"",
status:i||"success"
});
}
function initLikeEvent(opt){
function show(e){
e.style.display="";
}
function hide(e){
e.style.display="none";
}
function vShow(e){
e.style.visibility="visible";
}
function vHide(e){
e.style.visibility="hidden";
}
function clear(e){
e.value="";
}
function showLoading(){
commonUtils.isNativePage()?showAppToast("发送中","loading"):Loading.show("发送中");
}
function hideLoading(){
commonUtils.isNativePage()?showAppToast("","dismissloading"):Loading.hide();
}
function showToast(e){
commonUtils.isNativePage()?showAppToast(e):(el_toastMsg.innerHTML=e,show(el_likeToast),
setTimeout(function(){
hide(el_likeToast);
},1e3));
}
function alert2(e){
"5"!==window.item_show_type||!commonUtils.isNativePage()||Device.os.pc?window.weui.alert(e):alert(e);
}
function failAlert(e){
return e&&e.length>maxLikeCommentWord?void alert2("想法不可以超过%s字".replace("%s",maxLikeCommentWord)):void alert2("网络异常，请稍后重试");
}
function isAppCommentAvailable(){
return mmversion.isWechat?Device.os.ipad?!1:mmversion.isInMiniProgram?!1:mmversion.isIOS&&mmversion.gtVersion("7.0.8")?!0:mmversion.isAndroid&&mmversion.gtVersion("7.0.8")?!0:commonUtils.isNativePage()&&(mmversion.isIOS||mmversion.isAndroid)?!0:!1:!1;
}
var scrollTop,el_like=opt.likeAreaDom,el_likeNum=opt.likeNumDom,showType=opt.showType,prompted=opt.prompted,haokanLock=!1,startY,jumpWowLock=!1,el_likeToast=qs("js_like_toast"),el_likeBtn=qs("js_like_btn"),el_toastMsg=qs("js_toast_msg"),el_likeEducate=qs("js_like_educate"),el_friend_like=qs("js_friend_like_area"),el_go_wow=qs("js_go_wow"),el_likeComment=qs("js_like_comment"),el_bcommentPanel2=qs("js_comment_panel"),el_likeCommentShare=qs("js_like_comment_share"),el_likeCommentText=qs("js_comment_text"),el_commentCancel=qs("js_comment_cancel"),el_commentConfirm=qs("js_comment_confirm"),el_commentErrorMsg=qs("js_like_comment_msg"),el_commentCurrentCount=qs("js_like_current_cnt"),el_commentArea=qs("js_comment_area"),el_panelLikeTitle=qs("js_panel_like_title"),el_wowClosePanel=qs("wow_close_inform"),el_wowCloseAck=qs("wow_close_ack"),el_alertPanel=qs("js_alert_panel"),el_alertContent=qs("js_alert_content"),el_alertConfirm=qs("js_alert_confirm");
if(el_like&&el_likeNum){
window.appmsg_like_type&&2===window.appmsg_like_type?jsmonitorReport.setSum(114217,0,1):window.appmsg_like_type&&1===window.appmsg_like_type&&jsmonitorReport.setSum(114217,1,1);
var like_report=function(){
var e=el_like.getAttribute("like"),i=el_likeNum.innerHTML,t=parseInt(e)?parseInt(e):0,o=t?0:1,n=parseInt(i)?parseInt(i):0,s=opt.appmsgid||opt.mid,l=opt.itemidx||opt.idx;
if(t){
if(1!==appmsg_like_type)return void sendRecommendAjax(0);
Class.removeClass(el_like,opt.className),el_like.setAttribute("like",0),n>0&&"100000+"!==i&&(el_likeNum.innerHTML=n-1==0?"赞":n-1);
}else if(1===appmsg_like_type)el_like.setAttribute("like",1),Class.addClass(el_like,opt.className),
"100000+"!==i&&(el_likeNum.innerHTML=n+1);else if(2===appmsg_like_type)return void initRecommendPanel();
RetryAjax({
url:"/mp/appmsg_like?__biz="+opt.biz+"&mid="+opt.mid+"&idx="+opt.idx+"&like="+o+"&f=json&appmsgid="+s+"&itemidx="+l,
data:{
is_temp_url:opt.is_temp_url||0,
scene:window.source,
subscene:window.subscene,
appmsg_like_type:window.appmsg_like_type,
item_show_type:parseInt(window.item_show_type,10),
client_version:window.clientversion,
action_type:o?1:2,
device_type:window.devicetype,
exptype:window.exptype||"",
expsessionid:window.expsessionid||""
},
type:"POST"
});
},initRecommendPanel=function(){
sendRecommendAjax(1,"",1);
},isBeenUnvisible=function(e){
function i(){
return window.pageYOffset||document.documentElement.scrollTop||document.body.scrollTop||0;
}
return e.offsetTop+el_likeComment.offsetHeight-i()>=commonUtils.getInnerHeight()?!0:!1;
},disableMove=function(){
document.addEventListener("touchmove",preventMove,{
passive:!1
}),el_likeCommentText.addEventListener("touchstart",getTouchStart,{
passive:!1
}),el_likeCommentText.addEventListener("touchmove",preventText,!1);
},enableMove=function(){
document.removeEventListener("touchmove",preventMove,{
passive:!1
}),el_likeCommentText.removeEventListener("touchstart",getTouchStart,{
passive:!1
}),el_likeCommentText.removeEventListener("touchmove",preventText,!1);
},preventMove=function(e){
var i=e.target;
"TEXTAREA"!==i.tagName&&"BUTTON"!==i.tagName&&(e.preventDefault(),e.stopPropagation());
},getTouchStart=function(e){
var i=e.targetTouches||[];
if(i.length>0){
var t=i[0]||{};
startY=t.clientY;
}
},preventText=function(e){
var i=!1,t=e.changedTouches,o=this.scrollTop,n=this.offsetHeight,s=this.scrollHeight;
if(t.length>0){
var l=t[0]||{},a=l.clientY;
i=a>startY&&0>=o?!1:startY>a&&o+n>=s?!1:!0,i||e.preventDefault();
}
},isShow=function(e){
return"none"===e.style.display||"hidden"===e.style.visibility?!1:""===e.style.display||"block"===e.style.display||"visible"===e.style.visibility?!0:void 0;
},validataComment=function(e,i){
var t=e.value.replace(/^\s+|\s+$/g,"");
sendRecommendAjax(1,t,i);
},showEducatePanel=function(e,i,t){
show(el_likeComment);
var o=window.source||window.cgiData&&window.cgiData.source||0;
return o&&(o=parseInt(o,10),94===o)?void(e&&5===e&&hide(el_likeComment)):void(i||(show(el_likeEducate),
t&&t>0&&(el_friend_like.innerHTML="%s位朋友也在看,".replace("%s",t),document.getElementById("js_friend_like_word").innerText="前往“发现”-“看一看”浏览",
show(el_friend_like)),1===showType&&(hide(el_go_wow),hide(el_likeCommentShare)),
isBeenUnvisible(el_likeComment)&&scrollToShow(el_likeComment),educateExpose()));
},setBtnLike=function(){
el_like.setAttribute("like",1),Class.addClass(el_likeBtn,opt.className),realLikeNum+=1;
var e=el_likeNum.innerHTML;
"10万+"!==e&&(el_likeNum.innerHTML=formatReadNum(realLikeNum)),renderProfile(["zaikan"]);
},setLike2Status=function(e,i,t){
var o="在看 ";
switch(showType){
case 1:
switch(prompted){
case 0:
showEducatePanel(e,i,t),show(el_likeComment),prompted=1;
break;

case 1:
hide(el_likeEducate),showToast(o);
}
setBtnLike();
break;

case 2:
switch(hide(el_bcommentPanel2),clear(el_likeCommentText),prompted){
case 0:
showEducatePanel(e,i,t),5===e&&hide(el_likeCommentShare);
break;

case 1:
(4===e||5===e)&&showToast(4===e?"已发送":o);
}
5!==e&&(4===e&&"none"!==el_likeEducate.style.display?hide(el_likeCommentShare):4===e?hide(el_likeComment):(show(el_commentArea),
show(el_likeCommentShare),1===prompted&&hide(el_likeEducate),show(el_likeComment),
isBeenUnvisible(el_likeComment)&&scrollToShow(el_likeComment))),4!==e&&setBtnLike(),
prompted=1;
}
enableMove(),commonUtils.isNativePage()&&JSAPI.invoke("handleHaokanAction",{
action:"closeComment"
}),log("[Appmsg] zaikan set like success");
},unsetLike2Status=function(e){
1===e?setTimeout(function(){
alert2(" 已取消，想法已同步删除");
},20):showToast("已取消"),2===showType&&isShow(el_likeComment)&&hide(el_likeComment);
var i=el_likeNum.innerHTML;
Class.removeClass(el_likeBtn,opt.className),el_like.setAttribute("like",0),el_likeComment&&hide(el_likeComment),
realLikeNum-=1,realLikeNum>=0&&"10万+"!==i&&(el_likeNum.innerHTML=formatReadNum(realLikeNum)),
log("[Appmsg] zaikan set unlike success");
},sendRecommendAjax=function sendRecommendAjax(like,comment,type,clientType){
if(!haokanLock){
log("[Appmsg] prepare to send appmsg like request"),showLoading();
var appmsgid=opt.appmsgid||opt.mid,itemidx=opt.itemidx||opt.idx;
haokanLock=!0;
var action_type;
like?(window.isPaySubscribe&&payReportUtils.reportPayAppmsg(12),action_type=type):(window.isPaySubscribe&&payReportUtils.reportPayAppmsg(13),
action_type=2),ajax({
url:"/mp/appmsg_like?__biz="+opt.biz+"&mid="+opt.mid+"&idx="+opt.idx+"&like="+like+"&f=json&appmsgid="+appmsgid+"&itemidx="+itemidx,
data:{
is_temp_url:opt.is_temp_url||0,
scene:window.source,
subscene:window.subscene,
appmsg_like_type:window.appmsg_like_type,
item_show_type:parseInt(window.item_show_type,10),
client_version:window.clientversion,
comment:comment?comment:"",
prompted:1,
style:clientType||showType,
action_type:action_type,
passparam:window.passparam,
request_id:(new Date).getTime(),
device_type:window.devicetype,
exptype:window.exptype||"",
expsessionid:window.expsessionid||""
},
type:"POST",
success:function success(res){
haokanLock=!1;
var data=eval("("+res+")");
hideLoading(),log("[Appmsg] success send appmsglike like "+like+" return value is "+JSON.stringify(res)),
0==data.base_resp.ret?(like?(setLike2Status(type,data.is_eu_user,data.friend_like_num),
relatedArticles&&relatedArticles.render&&relatedArticles.render("like")):unsetLike2Status(data.has_comment),
connectWithApp(like,comment,clientType)):failAlert(comment);
},
error:function(){
hideLoading(),failAlert(),haokanLock=!1;
}
});
}
};
JSAPI.on("menu:haokan",function(e){
var i=0===parseInt(e.recommend)?0:1;
if(0===i)sendRecommendAjax(i,"",2,clientShowType);else{
var t="";
t=e.comment;
var o=1===e.scene?4:5;
sendRecommendAjax(i,t,o,clientShowType);
}
});
var connectWithApp=function(e,i){
var t={
origin:"mp",
isLike:e?1:0,
url:encodeURIComponent(msg_link.html(!1)),
content:i?i:""
};
JSAPI.invoke("handleHaokanAction",{
action:actionString,
recommend:e?1:0,
server_data:JSON.stringify(t)
},function(e){
console.log("handleHaokanAction",e);
}),JSAPI.invoke("handleHaokanAction",{
action:actionForClient,
permission:1,
recommend:e?1:0
},function(e){
console.log("handleHaokanAction for client",e);
});
},goWoW=function(){
jumpWowLock||(jumpToWowClickReport(),jumpWowLock=!0,JSAPI.invoke("handleHaokanAction",{
action:"jumpToWow",
extParams:JSON.stringify({
autoDropLoad:!0
})
},function(e){
jumpWowLock=!1,console.log("jumpToWow",e),e.err_msg&&"handleHaokanAction:fail_entrance_not_open"===e.err_msg?show(el_wowClosePanel):"handleHaokanAction:fail  action not support"===e.err_msg||"handleHaokanAction:fail, action not support"===e.err_msg?alert2("微信版本过低，暂不支持该操作"):"handleHaokanAction:ok"===e.err_msg&&hide(el_likeComment),
JSAPI.invoke("handleHaokanAction",{
action:actionString,
server_data:JSON.stringify({
origin:"mp",
autoDropLoad:!0
})
},function(e){
console.log("sendAutoDropLoad",e);
});
}));
},likeClickReport=function(){
ajax({
url:"/mp/appmsgreport?action=appmsglikeclickcomment&__biz="+opt.biz+"&mid="+opt.mid+"&idx="+opt.idx+"&f=json&appmsgid="+appmsgid+"&itemidx="+itemidx,
data:{
is_temp_url:opt.is_temp_url||0,
scene:window.source,
subscene:window.subscene,
appmsg_like_type:window.appmsg_like_type,
item_show_type:parseInt(window.item_show_type,10),
client_version:window.clientversion,
device_type:window.devicetype
},
type:"POST"
});
},likeExpose=function e(){
var i=document.documentElement.scrollTop||window.pageYOffset||document.body.scrollTop,t=qs("like3").offsetTop,o=opt.appmsgid||opt.mid,n=opt.itemidx||opt.idx;
i+commonUtils.getInnerHeight()>t&&t>=i&&(ajax({
url:"/mp/appmsgreport?action=appmsglikeexposure&__biz="+opt.biz+"&mid="+opt.mid+"&idx="+opt.idx+"&f=json&appmsgid="+o+"&itemidx="+n,
data:{
is_temp_url:opt.is_temp_url||0,
scene:window.source,
subscene:window.subscene,
appmsg_like_type:window.appmsg_like_type,
item_show_type:parseInt(window.item_show_type,10),
client_version:window.clientversion,
device_type:window.devicetype
},
type:"POST"
}),DomEvent.off(window,"scroll",e));
},educateExpose=function i(){
var e=(document.documentElement.scrollTop||window.pageYOffset||document.body.scrollTop,
opt.appmsgid||opt.mid),t=opt.itemidx||opt.idx,o=window.item_show_type,n=window.enterid||window.cgiData&&window.cgiData.enterid||"";
el_likeEducate&&"none"!=el_likeEducate.style.display&&commonUtils.getInnerHeight()>el_likeEducate.getBoundingClientRect().top&&el_likeEducate.getBoundingClientRect().top+el_likeEducate.getBoundingClientRect().height>0&&(ajax({
url:"/mp/webcommreport?action=report&report_useruin=1&__biz="+window.biz,
type:"POST",
data:{
logid:18266,
buffer:["",Base64.decode(opt.biz),e,t,window.source,window.subscene,1,o,sessionid,n]
},
async:!1,
timeout:2e3
}),DomEvent.off(window,"scroll",i));
},jumpToWowClickReport=function(){
var e=opt.appmsgid||opt.mid,i=opt.itemidx||opt.idx,t=window.enterid||window.cgiData&&window.cgiData.enterid||"";
ajax({
url:"/mp/webcommreport?action=report&report_useruin=1&__biz="+window.biz,
type:"POST",
data:{
logid:18266,
buffer:["",Base64.decode(opt.biz),e,i,window.source,window.subscene,2,window.item_show_type,sessionid,t]
},
async:!1,
timeout:2e3
});
};
DomEvent.on(el_alertConfirm,"click",function(){
el_alertPanel.style.display="none";
}),DomEvent.on(el_like,"click",function(e){
if(el_likeBtn.disabled!==!0){
if(window.is_temp_url)return void alert2("预览状态下无法操作");
var i=el_like.getBoundingClientRect();
return log("[Appmsg zaikan location] top: "+i.top+" left: "+i.left+" bottom: "+i.bottom+" right: "+i.right),
log("[Appmsg zaikan click] clientX: "+e.clientX+" clientY: "+e.clientY),e.currentTarget.classList.contains("js_disabled")?!1:(like_report(e),
!1);
}
}),DomEvent.on(el_wowCloseAck,"click",function(){
hide(el_wowClosePanel);
}),DomEvent.on(qs("js_mask_2"),"mousedown",function(){
hide(el_bcommentPanel2),clear(el_likeCommentText),vHide(el_commentErrorMsg),enableMove();
}),DomEvent.on(el_commentConfirm,"mousedown",function(){
validataComment(el_likeCommentText,4);
}),DomEvent.on(el_commentCancel,"mousedown",function(){
hide(el_bcommentPanel2),clear(el_likeCommentText),vHide(el_commentErrorMsg),enableMove();
}),DomEvent.on(el_likeCommentShare,"click",function(){
return commonUtils.isNativePage()?void JSAPI.invoke("handleHaokanAction",{
action:"writeComment",
style:"white"
}):(scrollTop=document.body.scrollTop||document.documentElement.scrollTop,1*window.item_show_type===10&&(el_panelLikeTitle.innerHTML=window.msg_title.replace(/<(\/?)(?=((a(\s|>))|(\/a))).*?>/g,"")),
show(el_bcommentPanel2),el_likeCommentText.focus(),el_commentConfirm.setAttribute("disabled","disabled"),
disableMove(),void likeClickReport());
}),DomEvent.on(el_likeCommentText,"focus",function(){}),DomEvent.on(el_likeCommentText,"blur",function(){
window.scrollTo(0,scrollTop);
}),DomEvent.on(window,"scroll",likeExpose),DomEvent.on(window,"scroll",educateExpose),
DomEvent.on(el_go_wow,"click",goWoW);
var scrollToShow=function(e){
e.scrollIntoView(!1);
};
DomEvent.on(el_likeCommentText,"input",function(e){
var i=el_likeCommentText.value.replace(/^\s+|\s+$/g,"");
i.length>maxLikeCommentWord?(el_commentCurrentCount.innerHTML=i.length,vShow(el_commentErrorMsg)):vHide(el_commentErrorMsg),
i.length>0&&i.length<=maxLikeCommentWord?el_commentConfirm.removeAttribute("disabled"):el_commentConfirm.setAttribute("disabled","disabled"),
Device.os.ios&&e.data&&doubleInputChar.indexOf(e.data)>-1&&(focusTag=!0);
}),DomEvent.on(el_likeCommentText,"click",function(){
Device.os.ios&&focusTag&&(el_likeCommentText.blur(),el_likeCommentText.focus(),focusTag=!1);
});
}
}
function showLikeNum(e){
var i=e||{};
if(i.show){
var t=i.likeAreaDom,o=i.likeNumDom,n=document.getElementById("js_like_btn");
t&&(t.style.display=i.likeAreaDisplayValue,t.style.visibility="",i.liked&&(1===appmsg_like_type?Class.addClass(t,i.className):Class.addClass(n,i.className)),
t.setAttribute("like",i.liked?"1":"0"),i.likeGray&&(n.disabled=!0));
var s=1===appmsg_like_type?"赞":"";
realLikeNum=i.likeNum||s,1===appmsg_like_type?(parseInt(realLikeNum)>1e5?realLikeNum="100000+":"",
o&&(o.innerHTML=realLikeNum)):2===appmsg_like_type&&(o.innerHTML=formatReadNum(realLikeNum));
}
}
var DomEvent=require("biz_common/dom/event.js"),Class=require("biz_common/dom/class.js"),ajax=require("biz_wap/utils/ajax.js"),Base64=require("biz_common/base64.js"),jsmonitorReport=require("biz_wap/utils/jsmonitor_report.js"),log=require("appmsg/log.js"),Tips=require("complain/tips.js"),RetryAjax=require("appmsg/retry_ajax.js"),JSAPI=require("biz_wap/jsapi/core.js"),actionString="submitMsgToTL",actionForClient="update_recommend_status",mmversion=require("biz_wap/utils/mmversion.js"),commonUtils=require("common/utils.js"),Loading=require("appmsg/loading.js"),_require=require("appmsg/like_and_share.js"),renderProfile=_require.renderProfile,realLikeNum,clientShowType=5,Device=require("biz_wap/utils/device.js"),payReportUtils=require("appmsg/pay_report_utils.js"),_require2=require("pages/utils.js"),formatReadNum=_require2.formatReadNum,relatedArticles=require("appmsg/related_article.js"),maxLikeCommentWord=200,focusTag=!1,doubleInputChar=["“”","‘’","（）","《》","〈〉","「」","『』","〔〕","【】","［］","[]","｛｝","{}","()","<>"];
return{
initLikeEvent:initLikeEvent,
showLikeNum:showLikeNum
};
});define("appmsg/read.js",["pages/utils.js","biz_wap/utils/device.js"],function(e){
"use strict";
function i(e){
var i=e||{},n=1586325600,d="undefined"!=typeof window.ct?parseInt(window.ct,10):0;
if(i.show){
var s=i.readAreaDom,o=i.readNumDom;
s&&(s.style.display=i.readAreaDisplayValue);
var r=i.readNum||1,w=window.ori_send_time||window.cgiData&&window.cgiData.ori_send_time||0,p=/(WindowsNT)|(Windows NT)|(Macintosh)/i.test(navigator.userAgent),m=1566025200,u=1565971200,_=a.os.ios||p?m:u;
parseInt(w,10)>_&&window.item_show_type&&"5"===window.item_show_type&&(n>d?("en"!==window.LANG&&(document.getElementById("readTxt").innerText="播放"),
r=i.videouv||0):("en"!==window.LANG&&(document.getElementById("readTxt").innerText="观看"),
r=i.readNum||0)),1===window.appmsg_like_type?(parseInt(r,10)>1e5?r="100000+":"",
o&&(o.innerHTML=r)):2===window.appmsg_like_type&&(o.innerHTML=t(r),""===o.innerHTML&&(o.innerHTML="0"));
}
}
var n=e("pages/utils.js"),t=n.formatReadNum,a=e("biz_wap/utils/device.js");
return{
showReadNum:i
};
});var _extends=Object.assign||function(o){
for(var e=1;e<arguments.length;e++){
var i=arguments[e];
for(var n in i)Object.prototype.hasOwnProperty.call(i,n)&&(o[n]=i[n]);
}
return o;
};
define("appmsg/like_profile.js",["biz_common/template-2.0.1-cmd.js","appmsg/like_profile_tpl.html.js","biz_wap/utils/ajax.js","biz_common/dom/event.js","biz_wap/jsapi/core.js","pages/utils.js","common/comm_report.js","biz_wap/utils/mmversion.js"],function(o){
"use strict";
function e(){
console.log(h),l.isWindows?location.href=h:c.invoke("profile",{
username:f,
scene:"209"
},function(){});
}
function i(o){
var i={
bottomOpr:m("js_bottom_opr_right"),
container:m("js_like_profile_container")
};
s({
url:"/mp/getbizbanner?__biz="+window.biz,
type:"GET",
dataType:"json",
success:function(s){
if(!s.base_resp||0===s.base_resp.ret){
var r=s.friend_subscribe_count,b=s.is_subscribed,k=s.orignal_num,z=s.is_ban;
if(!b&&!z){
_=!b,i.container.innerHTML=n.compile(t)({
friendSubscribeCount:r,
isSubscribed:b,
orignalNum:k,
isShowFocusBottom:_,
roundHeadImg:u,
nickname:w
}),o.forEach(function(o){
d.report(23219,_extends({},g,{
Actiontype:1,
type:j[o]
}));
}),m("js_like_profile_bar").classList.remove("wx_follow_hide");
var B=p("js_function_mod_inner")[0].offsetHeight;
p("js_function_mod")[0].style.height=B+"px",i.focusBottom=m("js_focus"),i.alreadyFocusBottom=m("js_already_focus"),
_?(i.focusBottom.style.display="block",i.alreadyFocusBottom.style.display="none"):(i.focusBottom.style.display="none",
i.alreadyFocusBottom.style.display="block"),a.on(i.focusBottom,"click",function(e){
e.stopPropagation(),o.forEach(function(o){
d.report(23219,_extends({},g,{
Actiontype:2,
type:j[o]
}));
}),l.isWindows?location.href=h:c.invoke("quicklyAddBrandContact",{
username:f,
scene:y
},function(o){
/ok/.test(o.err_msg)&&(i.focusBottom.style.display="none",i.alreadyFocusBottom.style.display="block");
});
}),a.on(i.alreadyFocusBottom,"click",function(i){
o.forEach(function(o){
d.report(23219,_extends({},g,{
Actiontype:4,
type:j[o]
}));
}),i.stopPropagation(),e();
}),a.on(m("js_like_profile_bar"),"click",function(){
o.forEach(function(o){
d.report(23219,_extends({},g,{
Actiontype:3,
type:j[o]
}));
}),e();
});
}
}
}
});
}
var n=o("biz_common/template-2.0.1-cmd.js"),t=o("appmsg/like_profile_tpl.html.js"),s=o("biz_wap/utils/ajax.js"),a=o("biz_common/dom/event.js"),c=o("biz_wap/jsapi/core.js"),r=o("pages/utils.js"),m=r.getId,p=r.getByClass,d=o("common/comm_report.js"),l=o("biz_wap/utils/mmversion.js"),_=!0,u=window.round_head_img||window.cgiData.round_head_img||"http://mmbiz.qpic.cn/mmbiz/a5icZrUmbV8p5jb6RZ8aYfjfS2AVle8URwBt8QIu6XbGewB9wiaWYWkPwq4R7pfdsFibuLkic16UcxDSNYtB8HnC1Q/0",w=window.nickname||window.cgiData.nick_name,f=window.user_name||window.cgiData.user_name,b=window.appuin||window.cgiData.biz,y=209,j={
share:1,
collect:2,
zaikan:3,
like:4
},g={
Msgid_from:1*(window.msgid||window.appmsgid),
Itemidx_from:1*window.idx,
Bizuin:window.biz,
Itemshowtype:1*window.item_show_type,
Sessioid:window.sessionid,
Enterid:1*window.enterid,
Scene:1*window.source,
Subscene:1*window.subscene
},h="https://mp.weixin.qq.com/mp/profile_ext?action=home&__biz="+b+"&scene=112#wechat_redirect";
return{
renderLikeProfile:i
};
});define("appmsg/share_tpl.html.js",[],function(){
return'<div class="rich_media_extra">\n    <a href="<#= url #>" class="share_appmsg_container appmsg_card_context flex_context">\n        <div class="flex_hd">\n            <i class="share_appmsg_icon"> </i>\n        </div>\n        <div class="flex_bd">\n            <div class="share_appmsg_title">分享给订阅用户</div>\n            <p class="share_appmsg_desc">可快速分享原创文章给你的公众号订阅用户</p>\n        </div>\n    </a>\n</div>\n';
});define("appmsg/appmsgext.js",["appmsg/log.js","biz_wap/utils/ajax.js","rt/appmsg/getappmsgext.rt.js","biz_common/utils/wxgspeedsdk.js","biz_common/utils/url/parse.js"],function(e){
"use strict";
function i(e){
var i={
biz:"",
appmsg_type:"",
mid:"",
sn:"",
album_id:"",
idx:"",
scene:"",
title:"",
ct:"",
abtest_cookie:"",
devicetype:"",
version:"",
is_need_ticket:0,
is_need_ad:0,
comment_id:"",
is_need_reward:0,
both_ad:0,
reward_uin_count:0,
send_time:"",
msg_daily_idx:"",
is_original:0,
is_only_read:0,
req_id:"",
pass_ticket:"",
is_temp_url:0,
more_read_type:0,
rtId:"",
rtKey:"",
appmsg_like_type:1,
related_video_sn:"",
vid:"",
is_pay_subscribe:0,
pay_subscribe_uin_count:0,
has_red_packet_cover:0,
related_video_num:e.isPublicRelatedVideo?10:5,
album_video_num:5,
onSuccess:function(){},
onError:function(){}
};
for(var o in e)e.hasOwnProperty(o)&&(i[o]=e[o]);
console.info("[(评论、点赞、赞赏) 发送请求]: ",new Date),t({
url:"/mp/getappmsgext?f=json&mock="+n.getQuery("mock"),
data:{
r:Math.random(),
__biz:i.biz,
appmsg_type:i.appmsg_type,
mid:i.mid,
sn:i.sn,
idx:i.idx,
scene:i.scene,
title:encodeURIComponent(i.title.htmlDecode()),
ct:i.ct,
abtest_cookie:i.abtest_cookie,
devicetype:i.devicetype.htmlDecode(),
version:i.version.htmlDecode(),
is_need_ticket:i.is_need_ticket,
is_need_ad:i.is_need_ad,
comment_id:i.comment_id,
is_need_reward:i.is_need_reward,
both_ad:i.both_ad,
reward_uin_count:i.is_need_reward?i.reward_uin_count:0,
send_time:i.send_time,
msg_daily_idx:i.msg_daily_idx,
is_original:i.is_original,
is_only_read:i.is_only_read,
req_id:i.req_id,
pass_ticket:i.pass_ticket,
is_temp_url:i.is_temp_url,
item_show_type:i.item_show_type,
tmp_version:1,
more_read_type:i.more_read_type,
appmsg_like_type:i.appmsg_like_type,
related_video_sn:i.related_video_sn,
related_video_num:i.related_video_num,
vid:i.vid,
is_pay_subscribe:i.is_pay_subscribe,
pay_subscribe_uin_count:i.pay_subscribe_uin_count,
has_red_packet_cover:i.has_red_packet_cover,
album_id:0x11fd1c7c75070000,
album_video_num:i.album_video_num,
cur_album_id:window.appmsg_album_info?window.appmsg_album_info.id:"",
is_public_related_video:i.isPublicRelatedVideo,
encode_info_by_base64:i.encodeInfoByBase64
},
type:"POST",
dataType:"json",
rtId:i.rtId,
rtKey:i.rtKey,
rtDesc:_,
async:!0,
success:function(e){
if(console.info("[(评论、点赞、赞赏) 响应请求]: ",new Date,e),s("[Appmsg] success get async data"),
"function"==typeof i.onSuccess&&i.onSuccess(e),e)try{
s("[Appmsg] success get async data, async data is: "+JSON.stringify(e));
}catch(t){}else s("[Appmsg] success get async data, async data is empty");
if(!d&&"5"===window.item_show_type){
var _=Date.now()-window.logs.pagetime.page_begin;
if(d=!0,Math.random()>.1)return;
a.saveSpeeds({
uin:window.uin,
pid:675,
speeds:[{
sid:29,
time:_
}]
}),a.send();
}
},
error:function(){
s("[Appmsg] error get async data, biz="+i.biz+", mid="+i.mid),"function"==typeof i.onError&&i.onError();
},
complete:function(){
"function"==typeof i.onComplete&&i.onComplete();
}
});
}
var s=e("appmsg/log.js"),t=e("biz_wap/utils/ajax.js"),_=e("rt/appmsg/getappmsgext.rt.js"),a=e("biz_common/utils/wxgspeedsdk.js"),n=e("biz_common/utils/url/parse.js"),d=void 0;
return{
getData:i
};
});define("appmsg/img_copyright_tpl.html.js",[],function(){
return'<span class="original_img_wrp">            \n    <span class="tips_global">来自: <#=source_nickname#></span>\n</span>    ';
});define("pages/video_ctrl.js",[],function(){
"use strict";
function i(i){
i=i||window;
var n=i.cgiData;
return n&&2==n.ori_status&&1==n.is_mp_video&&(n.nick_name||n.hit_username)?!0:!1;
}
function n(i){
return i=i||window,!1;
}
function t(){
return!1;
}
function e(){
return-1!=w.indexOf("&dd=1")?!1:"54"==r.appmsg_type?!1:!0;
}
function o(){
var i;
if(parent==window)i=window;else try{
{
r.__videoDefaultRatio;
}
i=r;
}catch(n){
i=window;
}
var t=i.__videoDefaultRatio||16/9;
return"54"==i.appmsg_type?t:t;
}
var r=window.withoutIframe?window:window.parent.window,w=window.location.href;
return{
showPauseTips:e,
showVideoLike:t,
showVideoDetail:n,
showReprint:i,
getRatio:o
};
});