ofxSMTP
=======

## Description

An SMTP Client (including SSL/TLS for Gmail, etc) for openFrameworks.

## Features

- When using this with Gmail, you may find it useful to create an [app-specific password](https://support.google.com/accounts/answer/185833?hl=en). You may also need to enable "[Less secure apps](https://support.google.com/accounts/answer/6010255?hl=en)", but try it without doing that first.

## Getting Started

To get started, generate the example project files using the openFrameworks [Project Generator](http://openframeworks.cc/learning/01_basics/how_to_add_addon_to_project/).

## Documentation

API documentation can be found here.

## Build Status

Linux, macOS [![Build Status](https://travis-ci.org/bakercp/ofxSMTP.svg?branch=master)](https://travis-ci.org/bakercp/ofxSMTP)

Visual Studio, MSYS [![Build status](https://ci.appveyor.com/api/projects/status/601qtpifl060q6mn/branch/master?svg=true)](https://ci.appveyor.com/project/bakercp/ofxsmtp/branch/master)

## Compatibility

The `stable` branch of this repository is meant to be compatible with the openFrameworks [stable branch](https://github.com/openframeworks/openFrameworks/tree/stable), which corresponds to the latest official openFrameworks release.

The `master` branch of this repository is meant to be compatible with the openFrameworks [master branch](https://github.com/openframeworks/openFrameworks/tree/master).

Some past openFrameworks releases are supported via tagged versions, but only `stable` and `master` branches are actively supported.

## Requirements
- ofxPoco (core addon)
- [ofxSSLManager](https://github.com/bakercp/ofxSSLManager)

## Versioning

This project uses Semantic Versioning, although strict adherence will only come into effect at version 1.0.0.

## Licensing

See `LICENSE.md`.

## Contributing

Pull Requests are always welcome, so if you make any improvements please feel free to float them back upstream :)

1. Fork this repository.
2. Create your feature branch (`git checkout -b my-new-feature`).
3. Commit your changes (`git commit -am 'Add some feature'`).
4. Push to the branch (`git push origin my-new-feature`).
5. Create new Pull Request.
