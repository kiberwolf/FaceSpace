var list_across0 = [
'_contents_xml.htm',
'_reference.xml',
'_index.xml',
'_search_xml.htm',
'_external.xml'
];
var list_up0 = [
'mat2cpp.xml',
'operation.xml',
'scalar_valued.xml',
'sum.xml'
];
var list_down3 = [
'license.xml',
'getstarted.xml',
'operation.xml',
'mat2cpp.hpp.xml',
'mat2cpp_ok.m.xml',
'mat2cpp_ok.cpp.xml',
'news.xml',
'library.xml',
'mztuni.xml'
];
var list_down2 = [
'elementwise.xml',
'matrixwise.xml',
'scalar_valued.xml',
'other.xml'
];
var list_down1 = [
'max.xml',
'min.xml',
'scalar.xml',
'sum.xml'
];
var list_down0 = [
'sum.cpp.xml'
];
var list_current0 = [
'sum.xml#Syntax',
'sum.xml#Matlab or Octave',
'sum.xml#Matlab or Octave.Example',
'sum.xml#C++',
'sum.xml#C++.Example',
'sum.xml#C++.Source'
];
function choose_across0(item)
{	var index          = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = list_across0[index-1];
}
function choose_up0(item)
{	var index          = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = list_up0[index-1];
}
function choose_down3(item)
{	var index          = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = list_down3[index-1];
}
function choose_down2(item)
{	var index          = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = list_down2[index-1];
}
function choose_down1(item)
{	var index          = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = list_down1[index-1];
}
function choose_down0(item)
{	var index          = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = list_down0[index-1];
}
function choose_current0(item)
{	var index          = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = list_current0[index-1];
}
