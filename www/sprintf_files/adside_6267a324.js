F.module("/static/lemma/ui/view/adside/adside.js",function(e,c){var d=e("/static/common/lib/tangram/base/base.js");var b=e("/static/common/ui/nslog/nslog.js");var g=e("/static/common/ui/jquery/jquery.js");function f(k,j){k=d.g(k);if(k){if(d.ie&&d.ie<7){d.setStyle(k,"position","absolute");var i=j.top;k.style.setExpression("top","eval((document.documentElement||document.body).scrollTop+"+i+") + 'px'");delete j.top}else{d.setStyle(k,"position","fixed")}d.setStyles(k,j)}}function h(){d.ajax.get("/cms/rc/adSideConfig.json?t="+ +new Date(),function(i){if(i&&i.statusText=="OK"&&i.responseText){var j=d.json.parse(i.responseText);if(j.pic&&j.lnk){d("#adLayerSide").css("background",'url("'+j.pic+'") no-repeat center').attr("href",j.lnk);f("adLayerSide",{top:"415"});if(!d.cookie.get("adLayerClosed")){d("#adLayerSide").show();d("#adFlash").show()}}}else{d("#adLayerSide").hide()}})}var a=0;d("#adLayerSide").click(function(){b(location.href,9213)});d("#adLayerSide .close").click(function(i){a=1;d("#adLayerSide").css("display","none");d("#adFlash").css("display","none");i.preventDefault();i.stopPropagation();d.cookie.set("adLayerClosed","1",{path:"/",expires:1000*24*3600})});c.init=h;return c},["/static/common/lib/tangram/base/base.js","/static/common/ui/nslog/nslog.js","/static/common/ui/jquery/jquery.js"]);