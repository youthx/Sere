"""
通用http协议处理
"""
from urllib import parse,request
import re

class net_work_util():

    @staticmethod
    def get_net_ip_address():
        """
        获取公网IP地址
        """
        url = request.urlopen("http://txt.go.sohu.com/ip/soip")
        text =str(url.read(),encoding='utf-8')
        print(text)
        ip = re.findall('\d+.\d+.\d+.\d+',text)
        return ip[0]

class UriTool():

    @staticmethod
    def url_query_replace(url:str,key:str,val:str,encoding='utf-8') -> str:
        """
        替换某个参数值 
        """
        start_uri = parse.urlparse(url)
        key = str(key)
        val = str(val)
        qs = parse.parse_qs(start_uri.query,keep_blank_values=True,encoding=encoding)
        qs[key]=(str(val),)
        new_query ='&'.join(map(lambda x :'{0}={1}'.format(x,parse.quote(qs[x][0],encoding=encoding)),qs))
        return parse.urlunparse((start_uri.scheme,start_uri.netloc,start_uri.path,start_uri.params,new_query,start_uri.fragment))
    
    @staticmethod
    def url_query_param(url:str,key:str,*,default_value:str='',encoding='utf-8') -> str:
        """
        获取链接中某个参数的值
        """
        key = str(key)
        default_value = str(default_value)
        start_uri = parse.urlparse(url)
        qs = parse.parse_qs(start_uri.query,keep_blank_values=True,encoding=encoding)
        param_value = qs.get(key)
        if param_value is None:
            return default_value
        return param_value[0]

    

    @staticmethod
    def url_query_remove(url:str,key:str,encoding='utf-8') -> str:
        """
        移除特定参数
        """
        start_uri = parse.urlparse(url)
        key = str(key)
        qs = parse.parse_qs(start_uri.query,keep_blank_values=True,encoding=encoding)
        if key in qs.keys():
            qs.pop(key)
        new_query ='&'.join(map(lambda x :'{0}={1}'.format(x,parse.quote(qs[x][0],encoding=encoding)),qs))
        return parse.urlunparse((start_uri.scheme,start_uri.netloc,start_uri.path,start_uri.params,new_query,start_uri.fragment))


if __name__ == "__main__":
    test_url = 'https://purcotton.tmall.com/i/asynSearch.htm?_ksTS=1571203927530_445&callback=jsonp446&mid=w-14440378953-0&wid=14440378953&path=/search.htm&search=y&orderType=null&viewType=grid&keyword=我们&lowPrice=null&highPrice=null#ffff'
    print(test_url)
    new_url = UriTool.url_query_replace(test_url,'pageNo','2')
    print(new_url)
    print(UriTool.url_query_param(test_url,'dfdfd'))
    print(UriTool.url_query_param(test_url,'dfdfd',default_value='dcccc'))
    print(UriTool.url_query_param(test_url,'page'))
    print(UriTool.url_query_param('https://list.jd.com/list.html?cat=1620,13780','page',default_value=0))
    pass