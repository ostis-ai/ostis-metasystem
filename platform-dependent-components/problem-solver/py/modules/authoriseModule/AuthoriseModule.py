from sc_kpm import ScModule

from .AuthoriseUserAgent import AuthoriseUserAgent


class AuthoriseModule(ScModule):
    def __init__(self):
        super().__init__(AuthoriseUserAgent())
