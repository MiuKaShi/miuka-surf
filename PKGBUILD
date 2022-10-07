# Maintainer: Antoine Stevan
pkgname=miuka-surf-git
pkgver=r6.ee86465
pkgrel=1
epoch=
pkgdesc="A heavily patched build of surf."
arch=(x86_64)
repo="miuka-surf"
url="https://github.com/MiuKaShi/miuka-surf.git"
license=('GNU')
groups=()
depends=(gcr webkit2gtk xorg-xprop)
makedepends=(git make)
checkdepends=()
optdepends=()
provides=(surf)
conflicts=(surf)
replaces=()
backup=()
options=()
install=
changelog=
source=("git+$url")
noextract=()
md5sums=('SKIP')
validpgpkeys=()

pkgver() {
  cd "${_pkgname}"
  printf "r%s.%s" "$(git -C "$repo" rev-list --count HEAD)" "$(git -C "$repo" rev-parse --short HEAD)"
}

build() {
  cd "$repo"
  make
}

package() {
  cd "$repo"
  mkdir -p ${pkgdir}/opt/${pkgname}
  echo ${pkgdir}
  echo ${pkgname}
  cp -rf * ${pkgdir}/opt/${pkgname}
  make PREFIX=/usr DESTDIR="${pkgdir}" install
  # install -Dm645 LICENSE "${pkgdir}/usr/share/licenses/${pkgname}/LICENSE"
  # install -Dm644 README.md "${pkgdir}/usr/share/doc/${pkgname}/README.md"
}
