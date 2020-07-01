#include <unistd.h>
#include <string.h>

#define LISHA_CA \
"-----BEGIN CERTIFICATE-----\r\n" \
"MIIHJDCCBgygAwIBAgIMOCjQzbMKxTQVjJ+bMA0GCSqGSIb3DQEBCwUAMIG8MQsw\r\n" \
"CQYDVQQGEwJCUjExMC8GA1UEChMoUmVkZSBOYWNpb25hbCBkZSBFbnNpbm8gZSBQ\r\n" \
"ZXNxdWlzYSAtIFJOUDEkMCIGA1UECxMbR2VyZW5jaWEgZGUgU2Vydmljb3MgKEdT\r\n" \
"ZXIpMRcwFQYDVQQHEw5SaW8gZGUgSmFuZWlybzEXMBUGA1UECBMOUmlvIGRlIEph\r\n" \
"bmVpcm8xIjAgBgNVBAMTGVJOUCBJQ1BFZHUgT1YgU1NMIENBIDIwMTkwHhcNMjAw\r\n" \
"MTIwMTQyNjM3WhcNMjIwMTIwMTQyNjM3WjB9MQswCQYDVQQGEwJCUjELMAkGA1UE\r\n" \
"CBMCU0MxFjAUBgNVBAcTDUZsb3JpYW5vcG9saXMxLzAtBgNVBAoTJlVOSVZFUlNJ\r\n" \
"REFERSBGRURFUkFMIERFIFNBTlRBIENBVEFSSU5BMRgwFgYDVQQDDA8qLmxpc2hh\r\n" \
"LnVmc2MuYnIwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQDyYqk2uGPt\r\n" \
"Ve5ndBBtjBwDvlY0OGCbI6xFJwGY6WWZ5BZLIU52SeOA6VSnCOIiGdMIsGodcbD9\r\n" \
"9LcvDkeQooddVfK+tghEZe1c+VCxLNE6JXeQtW771pETyWJb6ND4Y22JVLq1baVS\r\n" \
"uWX3Llsu6Dph30l34ATOX2gi545zH775gMLlbA/kDihpgYrO/CnTx4q8fMtq9YKG\r\n" \
"uHCMqG5BqtEpKlYlipgIbXJKtUVie0evGQdnt9HFquMl8bM+XKGEYM0Xlc/sPeZT\r\n" \
"Dgekfca7Wk9IDKlPjVtm4qhbPTAhwTrHsXfQ1WHv/NYXhoL0rhtwJts+hBTdmNwc\r\n" \
"2pRKmPWqUIa7AgMBAAGjggNiMIIDXjAOBgNVHQ8BAf8EBAMCBaAwgZcGCCsGAQUF\r\n" \
"BwEBBIGKMIGHMEgGCCsGAQUFBzAChjxodHRwOi8vc2VjdXJlLmdsb2JhbHNpZ24u\r\n" \
"Y29tL2NhY2VydC9ybnBpY3BlZHVvdnNzbGNhMjAxOS5jcnQwOwYIKwYBBQUHMAGG\r\n" \
"L2h0dHA6Ly9vY3NwLmdsb2JhbHNpZ24uY29tL3JucGljcGVkdW92c3NsY2EyMDE5\r\n" \
"MFYGA1UdIARPME0wQQYJKwYBBAGgMgEUMDQwMgYIKwYBBQUHAgEWJmh0dHBzOi8v\r\n" \
"d3d3Lmdsb2JhbHNpZ24uY29tL3JlcG9zaXRvcnkvMAgGBmeBDAECAjAJBgNVHRME\r\n" \
"AjAAMEMGA1UdHwQ8MDowOKA2oDSGMmh0dHA6Ly9jcmwuZ2xvYmFsc2lnbi5jb20v\r\n" \
"cm5waWNwZWR1b3Zzc2xjYTIwMTkuY3JsMCkGA1UdEQQiMCCCDyoubGlzaGEudWZz\r\n" \
"Yy5icoINbGlzaGEudWZzYy5icjAdBgNVHSUEFjAUBggrBgEFBQcDAQYIKwYBBQUH\r\n" \
"AwIwHwYDVR0jBBgwFoAUqzDHByhLy3qOW/5haffUerKFm0gwHQYDVR0OBBYEFK4M\r\n" \
"vnX5dPKTwTAz1fFXSg6qw40RMIIBfgYKKwYBBAHWeQIEAgSCAW4EggFqAWgAdQBv\r\n" \
"U3asMfAxGdiZAKRRFf93FRwR2QLBACkGjbIImjfZEwAAAW/DWTATAAAEAwBGMEQC\r\n" \
"IFPHz4Zl9H9prEkFHWBNTn/0lwZc4vT17Jp1XiQ0qZsNAiBJ2wenp6JEHLr1ysUM\r\n" \
"0YM9WSIUy4SC+7lB+agOQvoQ4gB3AO5Lvbd1zmC64UJpH6vhnmajD35fsHLYgwDE\r\n" \
"e4l6qP3LAAABb8NZMBgAAAQDAEgwRgIhAOSQvNu/0xh2Niqq7Kefw0wumt3Mi637\r\n" \
"1aAWG61tAWfeAiEA+lk9py1WAgE0w56fHBh4L68aNiWYURgL7G5mFP2mNvQAdgBV\r\n" \
"gdTCFpA2AUrqC5tXPFPwwOQ4eHAlCBcvo6odBxPTDAAAAW/DWS/yAAAEAwBHMEUC\r\n" \
"IQC5enpNDK+Fmkh/YbvYyAuKRJI8EOn/r9C8f/rtbyPSPQIgMfwTAQ7xK46aN6y0\r\n" \
"qr2IisCzOUga+ZqX0HlB9DbfnUMwDQYJKoZIhvcNAQELBQADggEBAFe9s6ZsHEBY\r\n" \
"h+EHqPzaa26xTVfKrxQC05S0lcd8YdOjvSpga+CLV7TeA4JqEzJSeoU1V5ZhlBbx\r\n" \
"10QgiTWwmZjx2rjKP1F5zi+vYlNNGiMe8E6AzN8hNqOZ022+hFtDDZBMyvCiBAoN\r\n" \
"tXjZ1g+K6ltse+rF3dx4KWvkdE/OlCsUx9wyqlg5BsHEETc+dMydtGM+E1+WdqBD\r\n" \
"yqWdDqtmPYQZc+3kEW6aiGvLtGUuJ4lnUUogQcf4ZZFBfR0GmAUR3GtNe8l2FSNY\r\n" \
"Q8roLRNceuwWTwwkzX0Dh1jypVJgXF48XNVORHUwl4IuuU63AFIVjAyFnwICpijT\r\n" \
"yJHvdrBtXHw=\r\n" \
"-----END CERTIFICATE-----\r\n" \
"-----BEGIN CERTIFICATE-----\r\n" \
"MIIFaDCCBFCgAwIBAgIQdR4+54VB4tfq7ONxbGDjnjANBgkqhkiG9w0BAQsFADBj\r\n" \
"MQswCQYDVQQGEwJCRTEVMBMGA1UECxMMVHJ1c3RlZCBSb290MRkwFwYDVQQKExBH\r\n" \
"bG9iYWxTaWduIG52LXNhMSIwIAYDVQQDExlUcnVzdGVkIFJvb3QgQ0EgU0hBMjU2\r\n" \
"IEcyMB4XDTE5MDUxNTAwMDAwMFoXDTI2MDUxNTAwMDAwMFowgbwxCzAJBgNVBAYT\r\n" \
"AkJSMTEwLwYDVQQKEyhSZWRlIE5hY2lvbmFsIGRlIEVuc2lubyBlIFBlc3F1aXNh\r\n" \
"IC0gUk5QMSQwIgYDVQQLExtHZXJlbmNpYSBkZSBTZXJ2aWNvcyAoR1NlcikxFzAV\r\n" \
"BgNVBAcTDlJpbyBkZSBKYW5laXJvMRcwFQYDVQQIEw5SaW8gZGUgSmFuZWlybzEi\r\n" \
"MCAGA1UEAxMZUk5QIElDUEVkdSBPViBTU0wgQ0EgMjAxOTCCASIwDQYJKoZIhvcN\r\n" \
"AQEBBQADggEPADCCAQoCggEBAKXy0aVSFMG4AXgSL5oDnUPqls7zPa1Fuik4KqTf\r\n" \
"STaz1Q7ucOLl++mgy+a0QsNCgunY3fXTwWmKMZABj4+K5OCzyNIuaAZdG7VN4+eu\r\n" \
"IWrHS6MDycpR31R/oz7Rle0bxv7mvEUBXoP/UIicYuM3jerIVCg6MsfvA9Zf96wW\r\n" \
"+Ay5wEmmGwhmNLSTULwKFLYzrxNoirAfgSFqOveSE4tjrpFuIuZYj/E587tpT14s\r\n" \
"PklK8iuQmbfe32JdsZ9kUKgAOjTnaFH3S1xaKAYsHHlOAlcond2TDneTAdU1B6fv\r\n" \
"fKHc8lWcGL1luzagXy8otoMTjLLAIMusMeGLKchPwUvHyrcCAwEAAaOCAbwwggG4\r\n" \
"MA4GA1UdDwEB/wQEAwIBhjAnBgNVHSUEIDAeBggrBgEFBQcDAQYIKwYBBQUHAwIG\r\n" \
"CCsGAQUFBwMJMBIGA1UdEwEB/wQIMAYBAf8CAQAwHQYDVR0OBBYEFKswxwcoS8t6\r\n" \
"jlv+YWn31HqyhZtIMB8GA1UdIwQYMBaAFMhjmwhpVMKYyNnN4zO3UF74yQGbMIGN\r\n" \
"BggrBgEFBQcBAQSBgDB+MDcGCCsGAQUFBzABhitodHRwOi8vb2NzcDIuZ2xvYmFs\r\n" \
"c2lnbi5jb20vdHJ1c3Ryb290c2hhMmcyMEMGCCsGAQUFBzAChjdodHRwOi8vc2Vj\r\n" \
"dXJlLmdsb2JhbHNpZ24uY29tL2NhY2VydC90cnVzdHJvb3RzaGEyZzIuY3J0MEEG\r\n" \
"A1UdHwQ6MDgwNqA0oDKGMGh0dHA6Ly9jcmwuZ2xvYmFsc2lnbi5jb20vZ3MvdHJ1\r\n" \
"c3Ryb290c2hhMmcyLmNybDBWBgNVHSAETzBNMEEGCSsGAQQBoDIBFDA0MDIGCCsG\r\n" \
"AQUFBwIBFiZodHRwczovL3d3dy5nbG9iYWxzaWduLmNvbS9yZXBvc2l0b3J5LzAI\r\n" \
"BgZngQwBAgIwDQYJKoZIhvcNAQELBQADggEBALoshiuq7NiX6GqigqN8pLfvz/ft\r\n" \
"29m6mEpkxzYZlyPIcsK5Vek4VNrucAhhttPl3HF85pNMDWICjpev4DwlQSs6q+H7\r\n" \
"fJG8we0PPIJyhVtYMTFbSZz8G7NQcuCeSeLlOjEkAWWLR0F/lhOTktURLoA3q6//\r\n" \
"lkkwEjOoQekpqI3RmmIMinWinIryzFim9P91ACY6O1oBi+kknw9V9vXHFyjnir6K\r\n" \
"729SJNjPshaIQ9OU8oTV7d88cXX5mPUDq6bac8Lql5jx+z7t2k/89x7E9IO9IpQY\r\n" \
"iSJjL2nmcKMip1/pHy1UOgI5UDQkouw8VCpi/TwwxybTPJIFOWPHt7GdN7o=\r\n" \
"-----END CERTIFICATE-----\r\n" \
"-----BEGIN CERTIFICATE-----\r\n" \
"MIIEXDCCA0SgAwIBAgILBAAAAAABNumCOV0wDQYJKoZIhvcNAQELBQAwTDEgMB4G\r\n" \
"A1UECxMXR2xvYmFsU2lnbiBSb290IENBIC0gUjMxEzARBgNVBAoTCkdsb2JhbFNp\r\n" \
"Z24xEzARBgNVBAMTCkdsb2JhbFNpZ24wHhcNMTIwNDI1MTEwMDAwWhcNMjcwNDI1\r\n" \
"MTEwMDAwWjBjMQswCQYDVQQGEwJCRTEVMBMGA1UECxMMVHJ1c3RlZCBSb290MRkw\r\n" \
"FwYDVQQKExBHbG9iYWxTaWduIG52LXNhMSIwIAYDVQQDExlUcnVzdGVkIFJvb3Qg\r\n" \
"Q0EgU0hBMjU2IEcyMIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAz80+\r\n" \
"/Q2PAhLuYwe04YTLBLGKr1/JScHtDvAY5E94GjGxCbSR1/1VhL880UPJyN85tddO\r\n" \
"oxZPgtIyZixDvvK+CgpT5webyBBbqK/ap7aoByghAJ7X520XZMRwKA6cEWa6tjCL\r\n" \
"WH1zscxQxGzgtV50rn2ux2SapoCPxMpM4+tpEVwWJf3KP3NT+jd9GRaXWgNei5JK\r\n" \
"Quo9l+cZkSeuoWijvaer5hcLCufPywMMQd0r6XXIM/l7g9DjMaE24d+fa2bWxQXC\r\n" \
"8WT/PZ+D1KUEkdtn/ixADqsoiIibGn7M84EE9/NLjbzPrwROlBUJFz6cuw+II0rZ\r\n" \
"8OFFeZ/OkHHYZq2h9wIDAQABo4IBJjCCASIwDgYDVR0PAQH/BAQDAgEGMA8GA1Ud\r\n" \
"EwEB/wQFMAMBAf8wRwYDVR0gBEAwPjA8BgRVHSAAMDQwMgYIKwYBBQUHAgEWJmh0\r\n" \
"dHBzOi8vd3d3Lmdsb2JhbHNpZ24uY29tL3JlcG9zaXRvcnkvMB0GA1UdDgQWBBTI\r\n" \
"Y5sIaVTCmMjZzeMzt1Be+MkBmzA2BgNVHR8ELzAtMCugKaAnhiVodHRwOi8vY3Js\r\n" \
"Lmdsb2JhbHNpZ24ubmV0L3Jvb3QtcjMuY3JsMD4GCCsGAQUFBwEBBDIwMDAuBggr\r\n" \
"BgEFBQcwAYYiaHR0cDovL29jc3AyLmdsb2JhbHNpZ24uY29tL3Jvb3RyMzAfBgNV\r\n" \
"HSMEGDAWgBSP8Et/qC5FJK5NUPpjmove4t0bvDANBgkqhkiG9w0BAQsFAAOCAQEA\r\n" \
"XzbLwBjJiY6j3WEcxD3eVnsIY4pY3bl6660tgpxCuLVx4o1xyiVkS/BcQFD7GIoX\r\n" \
"FBRrf5HibO1uSEOw0QZoRwlsio1VPg1PRaccG5C1sB51l/TL1XH5zldZBCnRYrrF\r\n" \
"qCPorxi0xoRogj8kqkS2xyzYLElhx9X7jIzfZ8dC4mgOeoCtVvwM9xvmef3n6Vyb\r\n" \
"7/hl3w/zWwKxWyKJNaF7tScD5nvtLUzyBpr++aztiyJ1WliWcS6W+V2gKg9rxEC/\r\n" \
"rc2yJS70DvfkPiEnBJ2x2AHZV3yKTALUqurkV705JledqUT9I5frAwYNXZ8pNzde\r\n" \
"n+DIcSIo7yKy6MX9czbFWQ==\r\n" \
"-----END CERTIFICATE-----\r\n" \
"-----BEGIN CERTIFICATE-----\r\n" \
"MIIDXzCCAkegAwIBAgILBAAAAAABIVhTCKIwDQYJKoZIhvcNAQELBQAwTDEgMB4G\r\n" \
"A1UECxMXR2xvYmFsU2lnbiBSb290IENBIC0gUjMxEzARBgNVBAoTCkdsb2JhbFNp\r\n" \
"Z24xEzARBgNVBAMTCkdsb2JhbFNpZ24wHhcNMDkwMzE4MTAwMDAwWhcNMjkwMzE4\r\n" \
"MTAwMDAwWjBMMSAwHgYDVQQLExdHbG9iYWxTaWduIFJvb3QgQ0EgLSBSMzETMBEG\r\n" \
"A1UEChMKR2xvYmFsU2lnbjETMBEGA1UEAxMKR2xvYmFsU2lnbjCCASIwDQYJKoZI\r\n" \
"hvcNAQEBBQADggEPADCCAQoCggEBAMwldpB5BngiFvXAg7aEyiie/QV2EcWtiHL8\r\n" \
"RgJDx7KKnQRfJMsuS+FggkbhUqsMgUdwbN1k0ev1LKMPgj0MK66X17YUhhB5uzsT\r\n" \
"gHeMCOFJ0mpiLx9e+pZo34knlTifBtc+ycsmWQ1z3rDI6SYOgxXG71uL0gRgykmm\r\n" \
"KPZpO/bLyCiR5Z2KYVc3rHQU3HTgOu5yLy6c+9C7v/U9AOEGM+iCK65TpjoWc4zd\r\n" \
"QQ4gOsC0p6Hpsk+QLjJg6VfLuQSSaGjlOCZgdbKfd/+RFO+uIEn8rUAVSNECMWEZ\r\n" \
"XriX7613t2Saer9fwRPvm2L7DWzgVGkWqQPabumDk3F2xmmFghcCAwEAAaNCMEAw\r\n" \
"DgYDVR0PAQH/BAQDAgEGMA8GA1UdEwEB/wQFMAMBAf8wHQYDVR0OBBYEFI/wS3+o\r\n" \
"LkUkrk1Q+mOai97i3Ru8MA0GCSqGSIb3DQEBCwUAA4IBAQBLQNvAUKr+yAzv95ZU\r\n" \
"RUm7lgAJQayzE4aGKAczymvmdLm6AC2upArT9fHxD4q/c2dKg8dEe3jgr25sbwMp\r\n" \
"jjM5RcOO5LlXbKr8EpbsU8Yt5CRsuZRj+9xTaGdWPoO4zzUhw8lo/s7awlOqzJCK\r\n" \
"6fBdRoyV3XpYKBovHd7NADdBj+1EbddTKJd+82cEHhXXipa0095MJ6RMG3NzdvQX\r\n" \
"mcIfeg7jLQitChws/zyrVQ4PkX4268NXSb7hLi18YIvDQVETI53O9zJrlAGomecs\r\n" \
"Mx86OyXShkDOOyyGeMlhLxS67ttVb9+E7gUJTb0o2HLO02JQZR7rkpeDMdmztcpH\r\n" \
"WD9f\r\n" \
"-----END CERTIFICATE-----\r\n"


#define LISHA \
"-----BEGIN CERTIFICATE-----\r\n" \
"MIIHJDCCBgygAwIBAgIMOCjQzbMKxTQVjJ+bMA0GCSqGSIb3DQEBCwUAMIG8MQsw\r\n" \
"CQYDVQQGEwJCUjExMC8GA1UEChMoUmVkZSBOYWNpb25hbCBkZSBFbnNpbm8gZSBQ\r\n" \
"ZXNxdWlzYSAtIFJOUDEkMCIGA1UECxMbR2VyZW5jaWEgZGUgU2Vydmljb3MgKEdT\r\n" \
"ZXIpMRcwFQYDVQQHEw5SaW8gZGUgSmFuZWlybzEXMBUGA1UECBMOUmlvIGRlIEph\r\n" \
"bmVpcm8xIjAgBgNVBAMTGVJOUCBJQ1BFZHUgT1YgU1NMIENBIDIwMTkwHhcNMjAw\r\n" \
"MTIwMTQyNjM3WhcNMjIwMTIwMTQyNjM3WjB9MQswCQYDVQQGEwJCUjELMAkGA1UE\r\n" \
"CBMCU0MxFjAUBgNVBAcTDUZsb3JpYW5vcG9saXMxLzAtBgNVBAoTJlVOSVZFUlNJ\r\n" \
"REFERSBGRURFUkFMIERFIFNBTlRBIENBVEFSSU5BMRgwFgYDVQQDDA8qLmxpc2hh\r\n" \
"LnVmc2MuYnIwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQDyYqk2uGPt\r\n" \
"Ve5ndBBtjBwDvlY0OGCbI6xFJwGY6WWZ5BZLIU52SeOA6VSnCOIiGdMIsGodcbD9\r\n" \
"9LcvDkeQooddVfK+tghEZe1c+VCxLNE6JXeQtW771pETyWJb6ND4Y22JVLq1baVS\r\n" \
"uWX3Llsu6Dph30l34ATOX2gi545zH775gMLlbA/kDihpgYrO/CnTx4q8fMtq9YKG\r\n" \
"uHCMqG5BqtEpKlYlipgIbXJKtUVie0evGQdnt9HFquMl8bM+XKGEYM0Xlc/sPeZT\r\n" \
"Dgekfca7Wk9IDKlPjVtm4qhbPTAhwTrHsXfQ1WHv/NYXhoL0rhtwJts+hBTdmNwc\r\n" \
"2pRKmPWqUIa7AgMBAAGjggNiMIIDXjAOBgNVHQ8BAf8EBAMCBaAwgZcGCCsGAQUF\r\n" \
"BwEBBIGKMIGHMEgGCCsGAQUFBzAChjxodHRwOi8vc2VjdXJlLmdsb2JhbHNpZ24u\r\n" \
"Y29tL2NhY2VydC9ybnBpY3BlZHVvdnNzbGNhMjAxOS5jcnQwOwYIKwYBBQUHMAGG\r\n" \
"L2h0dHA6Ly9vY3NwLmdsb2JhbHNpZ24uY29tL3JucGljcGVkdW92c3NsY2EyMDE5\r\n" \
"MFYGA1UdIARPME0wQQYJKwYBBAGgMgEUMDQwMgYIKwYBBQUHAgEWJmh0dHBzOi8v\r\n" \
"d3d3Lmdsb2JhbHNpZ24uY29tL3JlcG9zaXRvcnkvMAgGBmeBDAECAjAJBgNVHRME\r\n" \
"AjAAMEMGA1UdHwQ8MDowOKA2oDSGMmh0dHA6Ly9jcmwuZ2xvYmFsc2lnbi5jb20v\r\n" \
"cm5waWNwZWR1b3Zzc2xjYTIwMTkuY3JsMCkGA1UdEQQiMCCCDyoubGlzaGEudWZz\r\n" \
"Yy5icoINbGlzaGEudWZzYy5icjAdBgNVHSUEFjAUBggrBgEFBQcDAQYIKwYBBQUH\r\n" \
"AwIwHwYDVR0jBBgwFoAUqzDHByhLy3qOW/5haffUerKFm0gwHQYDVR0OBBYEFK4M\r\n" \
"vnX5dPKTwTAz1fFXSg6qw40RMIIBfgYKKwYBBAHWeQIEAgSCAW4EggFqAWgAdQBv\r\n" \
"U3asMfAxGdiZAKRRFf93FRwR2QLBACkGjbIImjfZEwAAAW/DWTATAAAEAwBGMEQC\r\n" \
"IFPHz4Zl9H9prEkFHWBNTn/0lwZc4vT17Jp1XiQ0qZsNAiBJ2wenp6JEHLr1ysUM\r\n" \
"0YM9WSIUy4SC+7lB+agOQvoQ4gB3AO5Lvbd1zmC64UJpH6vhnmajD35fsHLYgwDE\r\n" \
"e4l6qP3LAAABb8NZMBgAAAQDAEgwRgIhAOSQvNu/0xh2Niqq7Kefw0wumt3Mi637\r\n" \
"1aAWG61tAWfeAiEA+lk9py1WAgE0w56fHBh4L68aNiWYURgL7G5mFP2mNvQAdgBV\r\n" \
"gdTCFpA2AUrqC5tXPFPwwOQ4eHAlCBcvo6odBxPTDAAAAW/DWS/yAAAEAwBHMEUC\r\n" \
"IQC5enpNDK+Fmkh/YbvYyAuKRJI8EOn/r9C8f/rtbyPSPQIgMfwTAQ7xK46aN6y0\r\n" \
"qr2IisCzOUga+ZqX0HlB9DbfnUMwDQYJKoZIhvcNAQELBQADggEBAFe9s6ZsHEBY\r\n" \
"h+EHqPzaa26xTVfKrxQC05S0lcd8YdOjvSpga+CLV7TeA4JqEzJSeoU1V5ZhlBbx\r\n" \
"10QgiTWwmZjx2rjKP1F5zi+vYlNNGiMe8E6AzN8hNqOZ022+hFtDDZBMyvCiBAoN\r\n" \
"tXjZ1g+K6ltse+rF3dx4KWvkdE/OlCsUx9wyqlg5BsHEETc+dMydtGM+E1+WdqBD\r\n" \
"yqWdDqtmPYQZc+3kEW6aiGvLtGUuJ4lnUUogQcf4ZZFBfR0GmAUR3GtNe8l2FSNY\r\n" \
"Q8roLRNceuwWTwwkzX0Dh1jypVJgXF48XNVORHUwl4IuuU63AFIVjAyFnwICpijT\r\n" \
"yJHvdrBtXHw=\r\n" \
"-----END CERTIFICATE-----\r\n"

const char lisha_ca_crt[] = LISHA_CA;
const size_t lisha_ca_crt_len = sizeof(lisha_ca_crt);