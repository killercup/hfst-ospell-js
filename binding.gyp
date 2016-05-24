{
  "targets": [
    {
      "target_name": "hfst-ospell",
      "sources": [
        "lib/hfst-ospell/ospell.cc",
        "lib/hfst-ospell/ZHfstOspeller.cc",
        "lib/hfst-ospell/hfst-ol.cc",
        "lib/hfst-ospell/ZHfstOspellerXmlMetadata.cc",
        "src/hfst-ospell.cc",
      ],
      'defines': [
        'HAVE_DLFCN_H=1',
        'HAVE_GETOPT_H=1',
        'HAVE_INTTYPES_H=1',
        'HAVE_LIBARCHIVE=1',
        'HAVE_MALLOC=1',
        'HAVE_MEMORY_H=1',
        'HAVE_STDINT_H=1',
        'HAVE_STDLIB_H=1',
        'HAVE_STRINGS_H=1',
        'HAVE_STRING_H=1',
        'HAVE_STRNDUP=1',
        'HAVE_SYS_STAT_H=1',
        'HAVE_SYS_TYPES_H=1',
        'HAVE_TINYXML2=1',
        'HAVE_UNISTD_H=1',
        'PACKAGE="hfstospell"',
        'PACKAGE_BUGREPORT="hfst-bugs@helsinki.fi"',
        'PACKAGE_NAME="hfstospell"',
        'PACKAGE_STRING="hfstospell 0.4.1"',
        'PACKAGE_TARNAME="hfstospell"',
        'PACKAGE_URL="http://hfst.sf.net"',
        'PACKAGE_VERSION="0.4.1"',
        'STDC_HEADERS=1',
        'VERSION="0.4.1"',
        'ZHFST_EXTRACT_TO_MEM=1',
      ],
      "include_dirs" : [ 
          "<!(node -e \"require('nan')\")",
      ],
      "cflags": [
        "<!(pkg-config --cflags tinyxml2)",
        "<!(pkg-config --cflags libarchive)",

      ],
      "link_settings": {
        "libraries": [
          "<!(pkg-config --libs tinyxml2)",
          "<!(pkg-config --libs libarchive)",
        ],
      },
      
      # Enable exceptions
      'cflags!': [ '-fno-exceptions' ],
      'cflags_cc!': [ '-fno-exceptions' ],
      'conditions': [
        ['OS=="mac"', {
          'xcode_settings': {
            'GCC_ENABLE_CPP_EXCEPTIONS': 'YES'
          }
        }]
      ]
    }
  ]
}