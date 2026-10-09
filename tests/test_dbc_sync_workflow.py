"""Protect publication ordering and PR validation boundaries."""
from pathlib import Path
import yaml

ROOT = Path(__file__).resolve().parents[1]


def workflow(name):
    return yaml.load((ROOT / '.github/workflows' / name).read_text(), Loader=yaml.BaseLoader)


def test_sync_builds_before_publishing_and_pins_interface_commit():
    config = workflow('regenerate-dbcs.yml')
    steps = config['jobs']['sync-dbc']['steps']
    names = [step['name'] for step in steps]
    assert names.index('Build dashboard UI') < names.index('Publish interface PR')
    assert names.index('Pin published interfaces') < names.index('Publish dashboard PR')
    source = next(s for s in steps if s['name'] == 'Checkout DBC source')
    assert source['with']['repository'] == 'FSLART/T26_DBC'
    assert source['with']['path'] == 'build/dbc-source'
    interfaces = next(s for s in steps if s['name'] == 'Publish interface PR')
    assert interfaces['with']['path'] == 'build/interface-publish'
    publication = next(s for s in steps if s['name'] == 'Checkout interface publication repository')
    assert publication['with']['repository'] == 'FSLART/lart_msgs'
    assert publication['with']['ref'] == '${{ steps.baseline.outputs.sha }}'
    assert publication['with']['path'] == interfaces['with']['path']
    assert names.index('Copy tested interfaces') < names.index('Publish interface PR')
    assert interfaces['with']['base'] == 'main'
    assert 'git push' not in str(config)


def test_pull_requests_build_but_cannot_publish_releases():
    config = workflow('validate-dbc-arm64.yml')
    assert 'pull_request' in config['on']
    steps = config['jobs']['build-arm64']['steps']
    assert any('regenerate_dbcs.py' in s.get('run', '') for s in steps)
    releases = [s for s in steps if 'softprops/action-gh-release@' in s.get('uses', '')]
    assert not releases
    assert config['permissions']['contents'] == 'read'
